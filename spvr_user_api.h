//
// Created by RGAA on 31/10/2025.
//

#ifndef GAMMARAYPREMIUM_SPVR_USER_API_H
#define GAMMARAYPREMIUM_SPVR_USER_API_H

#include <string>
#include <vector>
#include <tuple>
#include <memory>

#include "expt/expected.h"
#include "spvr_errors.h"
#include "spvr_user.h"

namespace spvr
{

    class SpvrUserApi {
    public:
        // register
        static tc::Result<SpvrUserPtr, SpvrApiError> Register(const std::string& host,
                                                              int port,
                                                              const std::string& appkey,
                                                              const std::string& username,
                                                              const std::string& password);

        // login
        static tc::Result<SpvrUserPtr, SpvrApiError> Login(const std::string& host,
                                                           int port,
                                                           const std::string& appkey,
                                                           const std::string& uid);

        // logout
        static tc::Result<SpvrUserPtr, SpvrApiError> Logout(const std::string& host,
                                                            int port,
                                                            const std::string& appkey,
                                                            const std::string& uid);
    };

}

#endif //GAMMARAYPREMIUM_SPVR_USER_API_H
