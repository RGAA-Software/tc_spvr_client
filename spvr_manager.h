//
// Created by RGAA on 26/03/2025.
//

#ifndef GAMMARAY_GR_SPVR_MANAGER_H
#define GAMMARAY_GR_SPVR_MANAGER_H

#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "spvr_server_info.h"
#include "spvr_device_info.h"
#include "expt/expected.h"
#include "spvr_errors.h"

namespace tc
{

    class SpvrManager {
    public:
        SpvrManager();
        void SetHostPort(const std::string& host, int port);

        Result<std::shared_ptr<SpvrOnlineServers>, SpvrError> GetOnlineServers();
        // id has prefix, eg: server_xxxx
        Result<std::shared_ptr<SpvrDeviceInfo>, SpvrError> GetRelayDeviceInfo(const std::string& device_id);

    private:
        std::string host_{};
        int port_{0};
    };

}

#endif //GAMMARAY_GR_SPVR_MANAGER_H
