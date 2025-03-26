//
// Created by RGAA on 26/03/2025.
//

#ifndef GAMMARAY_SPVR_SERVER_INFO_H
#define GAMMARAY_SPVR_SERVER_INFO_H

#include <string>
#include <vector>

namespace tc
{

    class SpvrRelayServerInfo {
    public:


    };

    class SpvrProfileServerInfo {
    public:

    };

    class SpvrOnlineServers {
    public:
        std::vector<SpvrProfileServerInfo> pr_servers_;
        std::vector<SpvrRelayServerInfo> relay_servers_;
    };

}

#endif //GAMMARAY_SPVR_SERVER_INFO_H
