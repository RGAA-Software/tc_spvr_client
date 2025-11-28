//
// Created by RGAA on 28/11/2025.
//

#ifndef GAMMARAYPREMIUM_SPVR_USER_DEVICE_API_H
#define GAMMARAYPREMIUM_SPVR_USER_DEVICE_API_H

#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "expt/expected.h"
#include "spvr_errors.h"

namespace spvr
{

    class SpvrUserDevice;

    class SpvrUserDeviceApi {
    public:
        // query user-devices
        static
        tc::Result<std::vector<std::shared_ptr<SpvrUserDevice>>, SpvrApiError>
        QueryUserBindDevices(const std::string& host,
                             int port,
                             const std::string& appkey,
                             const std::string& uid,
                             int page,
                             int page_size);
    };

}
#endif //GAMMARAYPREMIUM_SPVR_USER_DEVICE_API_H
