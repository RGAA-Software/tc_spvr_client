//
// Created by RGAA on 26/03/2025.
//

#include "spvr_api.h"
#include "spvr_server_info.h"
#include "spvr_errors.h"
#include "json/json.hpp"
#include "tc_common_new/http_client.h"
#include "tc_common_new/log.h"
#include "tc_common_new/http_base_op.h"

using namespace nlohmann;

namespace spvr
{

    tc::Result<std::shared_ptr<SpvrOnlineServers>, SpvrError> SpvrApi::GetOnlineServers(const std::string& spvr_srv_host, int spvr_srv_port) {
        if (spvr_srv_host.empty() || spvr_srv_port <= 0) {
            return TRError(SpvrError::kSpvrRequestFailed);
        }
        auto client = tc::HttpClient::Make(spvr_srv_host, spvr_srv_port, kSpvrGetOnlineServers, 3000);
        auto resp = client->Request();
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("Request new device failed.");
            return TRError(SpvrError::kSpvrRequestFailed);
        }

        try {
            auto obj = json::parse(resp.body);
            if (obj["code"].get<int>() != 200) {
                return TRError(SpvrError::kSpvrJustCodeError);
            }

            auto data = obj["data"];
            if (!data.is_array()) {
                return TRError(SpvrError::kSpvrDataError);
            }

            auto online_servers = std::make_shared<SpvrOnlineServers>();
            bool settings_changed = false;
            for (const auto& item : data) {
                auto srv_type = item["server_type"].get<std::string>();
                auto srv_name = item["server_name"].get<std::string>();
                auto srv_id = item["server_id"].get<std::string>();
                auto srv_w3c_ip = item["w3c_ip"].get<std::string>();
                auto srv_local_ip = item["local_ip"].get<std::string>();
                auto srv_working_port = item["working_port"].get<std::string>();
                auto srv_grpc_port = item["grpc_port"].get<std::string>();
                if (srv_type == "0") {
                    // relay server
                    auto r = tc::HttpBaseOp::CanPingServer(srv_w3c_ip, srv_working_port);
                    LOGI("Ping relay server result: {}", r.has_value());
                    if (r) {
                        online_servers->relay_servers_.push_back(SpvrRelayServerInfo {
                            .srv_type_ = srv_type,
                            .srv_name_ = srv_name,
                            .srv_id_ = srv_id,
                            .srv_w3c_ip_ = srv_w3c_ip,
                            .srv_local_ip_ = srv_local_ip,
                            .srv_working_port_ = srv_working_port,
                            .srv_grpc_port_ = srv_grpc_port,
                        });
                        settings_changed = true;
                    }
                }
                else if (srv_type == "1") {
                    // profile server ; check it
                    auto r = tc::HttpBaseOp::CanPingServer(srv_w3c_ip, srv_working_port);
                    LOGI("Ping profile server result: {}", r.has_value());
                    // save to db
                    if (r) {
                        online_servers->pr_servers_.push_back(SpvrProfileServerInfo {
                            .srv_type_ = srv_type,
                            .srv_name_ = srv_name,
                            .srv_id_ = srv_id,
                            .srv_w3c_ip_ = srv_w3c_ip,
                            .srv_local_ip_ = srv_local_ip,
                            .srv_working_port_ = srv_working_port,
                            .srv_grpc_port_ = srv_grpc_port,
                        });
                        settings_changed = true;
                    }
                }

                LOGI("--online server : {}, type: {}", srv_name, srv_type);
                LOGI("----srv w3c ip: {}", srv_w3c_ip);
                LOGI("----srv local ip: {}", srv_local_ip);
                LOGI("----srv id: {}", srv_id);
                LOGI("----srv working port: {}", srv_working_port);
                LOGI("----srv grpc port: {}", srv_grpc_port);
            }

            if (settings_changed) {
                return online_servers;
            } else {
                return TRError(SpvrError::kSpvrNoOnlineServers);
            }
        } catch(std::exception& e) {
            LOGE("RequestNewDevice failed: {}, message: {}", e.what(), resp.body);
            return TRError(SpvrError::kSpvrParseJsonFailed);
        }
    }

}
