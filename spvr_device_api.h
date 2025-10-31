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
    // class
    class SpvrDevice;

    //
    using SpvrDevicePtr = std::shared_ptr<SpvrDevice>;

    class SpvrDeviceApi {
    public:
        // ping
        static tc::Result<bool, SpvrApiError> Ping(const std::string& host, int port, const std::string& appkey);

        // create new device
        static tc::Result<SpvrDevicePtr, SpvrApiError> RequestNewDevice(const std::string& host, int port, const std::string& appkey, const std::string& info);

        // update random password
        static tc::Result<SpvrDevicePtr, SpvrApiError> UpdateRandomPwd(const std::string& host, int port, const std::string& appkey, const std::string& device_id);

        // update safety password
        static tc::Result<SpvrDevicePtr, SpvrApiError> UpdateSafetyPwd(const std::string& host, int port, const std::string& appkey, const std::string& device_id, const std::string& safety_pwd_md5);

        // get device
        static tc::Result<SpvrDevicePtr, SpvrApiError> QueryDevice(const std::string& host, int port, const std::string& appkey, const std::string& device_id);

    private:
        static SpvrDevicePtr ParseJsonAsDevice(const std::string& body);

    };

}

#endif //GAMMARAY_GR_SPVR_MANAGER_H
