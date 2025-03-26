//
// Created by RGAA on 26/03/2025.
//

#include "spvr_manager.h"
#include "spvr_api.h"
#include "spvr_server_info.h"

namespace tc
{

    SpvrManager::SpvrManager() {

    }

    void SpvrManager::SetHostPort(const std::string& host, int port) {
        host_ = host;
        port_ = port;

        auto o = GetOnlineServers();
        if (o.has_value()) {

        }
    }

    Result<SpvrOnlineServers, std::string> SpvrManager::GetOnlineServers() {
        return Err<SpvrOnlineServers>("");
    }

    Result<SpvrDeviceInfo, std::string> SpvrManager::GetDeviceInfo() {
        return TRError("");
    }

}
