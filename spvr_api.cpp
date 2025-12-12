//
// Created by RGAA on 12/12/2025.
//

#include "spvr_api.h"

#include "spvr_device.h"
#include "tc_common_new/log.h"
#include "tc_common_new/http_client.h"
#include "tc_common_new/http_base_op.h"
#include "json/json.hpp"

using namespace tc;

namespace spvr
{

    const std::string kSpvrControl = "/api/v1/spvr/control";
    const std::string kQueryAliveConnections = kSpvrControl + "/query/alive/connections";
    const std::string kQueryAvailableNewConnection = kSpvrControl + "/available/new/connection";

    tc::Result<AliveConnections, SpvrApiError>
    SpvrApi::QueryAliveConnections(const std::string& host,
                                   int port,
                                   const std::string& appkey) {
        const auto client = HttpClient::MakeSSL(host, port, kQueryAliveConnections, 2000);
        auto resp = client->Request({
            {"appkey", appkey},
        });

        if (resp.status != 200 || resp.body.empty()) {
            LOGE("QueryUserDevices failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        try {
            json obj = json::parse(resp.body)[kData];
            int total = obj["total"].get<int>();
            int relay = obj["relay"].get<int>();
            return AliveConnections {
                .total_ = total,
                .relay_ = relay,
            };
        }
        catch (const std::exception& e) {
            LOGE("QueryUserBindDevices parse failed: {}", e.what());
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    tc::Result<AvailableNewConnection, SpvrApiError>
    SpvrApi::QueryAvailableNewConnection(const std::string& host, int port, const std::string& appkey) {
        const auto client = HttpClient::MakeSSL(host, port, kQueryAvailableNewConnection, 2000);
        auto resp = client->Request({
            {"appkey", appkey},
        });

        if (resp.status != 200 || resp.body.empty()) {
            LOGE("QueryUserDevices failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        try {
            json obj = json::parse(resp.body)[kData];
            auto available = obj["available"].get<bool>();
            return AvailableNewConnection {
                .available_ = available,
            };
        }
        catch (const std::exception& e) {
            LOGE("QueryUserBindDevices parse failed: {}", e.what());
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

}