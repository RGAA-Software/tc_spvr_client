//
// Created by RGAA on 26/03/2025.
//

#include "spvr_manager.h"
#include "spvr_api.h"
#include "spvr_server_info.h"
#include "spvr_errors.h"
#include "json/json.hpp"
#include "tc_common_new/http_client.h"
#include "tc_common_new/log.h"
#include "tc_common_new/http_base_op.h"

using namespace nlohmann;

namespace tc
{

    SpvrManager::SpvrManager() {

    }

    void SpvrManager::SetHostPort(const std::string& host, int port) {
        host_ = host;
        port_ = port;
    }

    Result<SpvrOnlineServers, SpvrError> SpvrManager::GetOnlineServers() {
        auto client =
                HttpClient::Make(std::format("{}:{}", host_, port_), kSpvrGetOnlineServers, 3);
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

            SpvrOnlineServers online_servers;
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
                    auto r = HttpBaseOp::CanPingServer(srv_w3c_ip, srv_working_port);
                    LOGI("Ping relay server result: {}", r.has_value());
                    if (r) {
                        online_servers.relay_servers_.push_back(SpvrRelayServerInfo {
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
                    auto r = HttpBaseOp::CanPingServer(srv_w3c_ip, srv_working_port);
                    LOGI("Ping profile server result: {}", r.has_value());
                    // save to db
                    if (r) {
                        online_servers.pr_servers_.push_back(SpvrProfileServerInfo {
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

    Result<SpvrDeviceInfo, SpvrError> SpvrManager::GetDeviceInfo(const std::string& device_id) {
        auto client =
                HttpClient::Make(std::format("{}:{}", host_, port_), kSpvrGetDeviceInfo, 3);
        auto resp = client->Request({
            {"device_id", device_id},
        });
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("Request new device failed.");
            return TRError(SpvrError::kSpvrRequestFailed);
        }

        try {
            auto obj = json::parse(resp.body);
            if (obj["code"].get<int>() != 200) {
                LOGE("GetDevice info failed: {}", obj["code"]);
                return TRError(SpvrError::kSpvrJustCodeError);
            }
            // "device_local_ips": "10.0.0.16;192.168.56.1;",
            // "relay_server_ip": "39.71.84.236",
            // "device_w3c_ip": "39.71.84.236",
            // "device_id": "server_488050198",
            // "relay_server_port": "40302"
            auto device_w3c_ip = obj["data"]["device_w3c_ip"].get<std::string>();
            auto relay_server_ip = obj["data"]["relay_server_ip"].get<std::string>();
            auto relay_server_port = std::atoi(obj["data"]["relay_server_port"].get<std::string>().c_str());

            return SpvrDeviceInfo {
                .device_w3c_ip_ = device_w3c_ip,
                .device_id_ = device_id,
                .relay_server_ip_ = relay_server_ip,
                .relay_server_port_ = relay_server_port,
            };
        } catch(...) {
            return TRError(SpvrError::kSpvrParseJsonFailed);
        }
    }

}
