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

    // ping
    const std::string kSpvrPing = "/ping";

    // /api/v1/device/control
    const std::string kSpvrDeviceControl = "/api/v1/device/control";

    // create new device
    const std::string kApiRequestNewDevice = kSpvrDeviceControl + "/create/new/device";

    // update random password
    const std::string kApiUpdateRandomPwd = kSpvrDeviceControl + "/update/random/pwd";

    // update safety password
    const std::string kApiUpdateSafetyPwd = kSpvrDeviceControl + "/update/safety/pwd";

    // get device by id
    const std::string kApiQueryDeviceById = kSpvrDeviceControl + "/query/device/by/id";

    // class
    class SpvrDevice;

    //
    using SpvrDevicePtr = std::shared_ptr<SpvrDevice>;

    class SpvrApi {
    public:
        // ping
        static tc::Result<bool, SpvrApiError> Ping(const std::string& host, int port, const std::string& appkey);
        // create new device
        static tc::Result<std::shared_ptr<SpvrDevice>, SpvrApiError> RequestNewDevice(const std::string& host, int port, const std::string& appkey, const std::string& info);
        // update random password
        static tc::Result<std::shared_ptr<SpvrDevice>, SpvrApiError> UpdateRandomPwd(const std::string& host, int port, const std::string& appkey, const std::string& device_id);
        // update safety password
        static tc::Result<std::shared_ptr<SpvrDevice>, SpvrApiError> UpdateSafetyPwd(const std::string& host, int port, const std::string& appkey, const std::string& device_id, const std::string& safety_pwd_md5);
        // get device
        static tc::Result<std::shared_ptr<SpvrDevice>, SpvrApiError> QueryDevice(const std::string& host, int port, const std::string& appkey, const std::string& device_id);

    private:
        static std::shared_ptr<SpvrDevice> ParseJsonAsDevice(const std::string& body);
    };

}

#endif //GAMMARAY_GR_SPVR_MANAGER_H
