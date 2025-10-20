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
#include "expt/expected.h"
#include "spvr_errors.h"

namespace spvr
{

    const std::string kSpvrGetOnlineServers = "/get/online/servers";
    const std::string kSpvrPing = "/ping";

    class SpvrApi {
    public:
        //static tc::Result<std::shared_ptr<SpvrOnlineServers>, SpvrError>
        //        GetOnlineServers(const std::string& spvr_srv_host, int spvr_srv_port);

        static tc::Result<bool, int> Ping(const std::string& host, int port, const std::string& appkey);

    };

}

#endif //GAMMARAY_GR_SPVR_MANAGER_H
