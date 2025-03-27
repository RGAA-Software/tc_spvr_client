//
// Created by RGAA on 26/03/2025.
//

#ifndef GAMMARAY_SPVR_DEVICE_INFO_H
#define GAMMARAY_SPVR_DEVICE_INFO_H

#include <string>
#include <vector>

namespace tc
{

    // "device_local_ips": "10.0.0.16;192.168.56.1;",
    // "relay_server_ip": "39.71.84.236",
    // "device_w3c_ip": "39.71.84.236",
    // "device_id": "server_488050198",
    // "relay_server_port": "40302"
    class SpvrDeviceInfo {
    public:
        std::vector<std::string> device_local_ips_;
        std::string device_w3c_ip_;
        int device_local_port_ = 0;

        std::string device_id_;
        std::string relay_server_ip_;
        int relay_server_port_ = 0;
    };

}

#endif //GAMMARAY_SPVR_DEVICE_INFO_H
