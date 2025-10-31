//
// Created by RGAA on 31/10/2025.
//

#include "spvr_user.h"
#include "json/json.hpp"
#include "tc_common_new/log.h"

using namespace nlohmann;

namespace spvr
{

    std::shared_ptr<SpvrUser> SpvrUser::FromJson(const std::string& json_str) {
        LOGI("Will parse json: {}", json_str);
        auto obj = json::parse(json_str);
        auto user = std::make_shared<SpvrUser>();
        user->uid_ = obj[kUserId].get<std::string>();
        user->username_ = obj[kUserName].get<std::string>();
        user->password_ = obj[kUserPassword].get<std::string>();
        user->assigned_ = obj[kUserAssigned].get<bool>();
        user->created_timestamp_ = obj[kUserCreatedTimestamp].get<int64_t>();
        user->updated_timestamp_ = obj[kUserUpdateTimestamp].get<int64_t>();
        user->deleted_ = obj[kUserDeleted].get<bool>();
        user->avatar_path_ = obj[kUserAvatarPath].get<std::string>();
        user->administrator_ = obj[kUserAdministrator].get<bool>();
        return user;
    }

    std::string SpvrUser::AsJson() {
        json obj;
        return obj.dump();
    }

}