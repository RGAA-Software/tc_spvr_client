//
// Created by RGAA on 26/03/2025.
//

#ifndef GAMMARAY_GR_SPVR_MANAGER_H
#define GAMMARAY_GR_SPVR_MANAGER_H

#include <string>
#include <vector>
#include <tuple>

#include "spvr_server_info.h"
#include "spvr_device_info.h"
#include "expt/expected.h"

namespace tc
{

    class SpvrManager {
    public:
        SpvrManager();
        void SetHostPort(const std::string& host, int port);

        Result<SpvrOnlineServers, std::string> GetOnlineServers();
        Result<SpvrDeviceInfo, std::string> GetDeviceInfo();

    private:
        std::string host_{};
        int port_{0};
    };

}

#endif //GAMMARAY_GR_SPVR_MANAGER_H
