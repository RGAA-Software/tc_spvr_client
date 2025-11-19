//
// Created by RGAA on 31/10/2025.
//

#include "spvr_user_api.h"
#include "json/json.hpp"
#include "tc_common_new/log.h"
#include "tc_common_new/http_client.h"
#include "tc_common_new/http_base_op.h"
#include "tc_common_new/thread.h"
#include "tc_common_new/message_notifier.h"
#include "tc_common_new/hardware.h"
#include "tc_common_new/ip_util.h"
#include "tc_common_new/base64.h"
#include "tc_common_new/uuid.h"

// /api/v1/user/control
const std::string kSpvrUserControl = "/api/v1/user/control";

// register
const std::string kRegister = kSpvrUserControl + "/register";

// login
const std::string kLogin = kSpvrUserControl + "/login";

// logout
const std::string kLogout = kSpvrUserControl + "/logout";

// delete
const std::string kDelete = kSpvrUserControl + "/delete";

// active
const std::string kActive = kSpvrUserControl + "/active";

// update avatar
const std::string kUpdateAvatar = kSpvrUserControl + "/update/avatar";

// update password
const std::string kUpdatePassword = kSpvrUserControl + "/update/password";

// query user by id
const std::string kQueryUserById = kSpvrUserControl + "/query/user/by/id";

// query user by name
const std::string kQueryUserByName = kSpvrUserControl + "/query/user/by/name";

// query users
const std::string kQueryUsers = kSpvrUserControl + "/query/users";

using namespace nlohmann;

namespace spvr
{

    // register
    tc::Result<SpvrUserPtr, SpvrApiError> SpvrUserApi::Register(const std::string& host,
                                                                int port,
                                                                const std::string& appkey,
                                                                const std::string& username,
                                                                const std::string& hash_password) {
        auto client = tc::HttpClient::MakeSSL(host, port, kRegister);

        json obj;
        obj[kUserName] = username;
        obj[kUserPassword] = hash_password;

        auto resp = client->Post({
            {"appkey", appkey}
        }, obj.dump());

        LOGI("Register, status:{}, : {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("Register failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        try {
            auto data = to_string(json::parse(resp.body)["data"]);
            return SpvrUser::FromJson(data);
        }
        catch(std::exception& e) {
            LOGE("Parse json failed: {}", e.what());
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    // login
    tc::Result<SpvrUserPtr, SpvrApiError> SpvrUserApi::Login(const std::string& host,
                                                             int port,
                                                             const std::string& appkey,
                                                             const std::string& username,
                                                             const std::string& hash_password) {
        auto client = tc::HttpClient::MakeSSL(host, port, kLogin);

        json obj;
        obj[kUserName] = username;
        obj[kUserPassword] = hash_password;

        auto resp = client->Post({
             {"appkey", appkey}
        }, obj.dump());

        LOGI("Login, status:{}, : {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("Register failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        try {
            auto data = to_string(json::parse(resp.body)["data"]);
            return SpvrUser::FromJson(data);
        }
        catch(std::exception& e) {
            LOGE("Parse json failed: {}", e.what());
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    // logout
    tc::Result<SpvrUserPtr, SpvrApiError> SpvrUserApi::Logout(const std::string& host,
                                                              int port,
                                                              const std::string& appkey,
                                                              const std::string& uid,
                                                              const std::string& hash_password) {
        return TcErr((SpvrApiError)0);
    }

}