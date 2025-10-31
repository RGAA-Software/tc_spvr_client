//
// Created by RGAA on 26/03/2025.
//

#include "spvr_device_api.h"
#include "spvr_server_info.h"
#include "spvr_errors.h"
#include "json/json.hpp"
#include "spvr_device.h"
#include "tc_common_new/http_client.h"
#include "tc_common_new/log.h"
#include "tc_common_new/http_base_op.h"
#include "tc_common_new/thread.h"
#include "tc_common_new/message_notifier.h"
#include "tc_common_new/hardware.h"
#include "tc_common_new/ip_util.h"
#include "tc_common_new/base64.h"
#include "tc_common_new/uuid.h"

using namespace tc;
using namespace nlohmann;

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

namespace spvr
{

    // Ping
    tc::Result<bool, SpvrApiError> SpvrDeviceApi::Ping(const std::string& host, int port, const std::string& appkey) {
        auto client = HttpClient::MakeSSL(host, port, kSpvrPing, 3000);
        auto resp = client->Request({
            {"appkey", appkey}
        });
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("GetRelayDeviceInfo failed : {}", resp.status);
            return TRError((SpvrApiError)resp.status);
        }

        try {
            auto obj = json::parse(resp.body);
            auto code = obj["code"].get<int>();
            auto data = obj["data"].get<std::string>();
            return code == 200 && data == "Pong";
        }
        catch (const std::exception& e) {
            LOGE("Ping Exception: {}, body: {}", e.what(), resp.body);
            return TRError(SpvrApiError::kParseJsonFailed);
        }
    }

    Result<SpvrDevicePtr, SpvrApiError> SpvrDeviceApi::RequestNewDevice(const std::string& host,
                                                                  int port,
                                                                  const std::string& appkey,
                                                                  const std::string& info) {
        std::string hw_info;
        if (info.empty()) {
            auto hardware_desc = Hardware::Instance()->GetHardwareDescription();
            auto et_info = IPUtil::ScanIPs();
            std::string mac_address;
            for (auto &item: et_info) {
                if (!item.mac_address_.empty() && mac_address.find(item.mac_address_) != std::string::npos) {
                    continue;
                }
                mac_address = mac_address.append(item.mac_address_);
            }
            if (hardware_desc.empty()) {
                LOGW("Hardware desc is empty! Can't request new device!");
            }
            hw_info = Base64::Base64Encode(hardware_desc + mac_address);
        }
        else {
            hw_info = info;
        }

        // SHIT!
        if (hw_info.empty()) {
            hw_info = GetUUID();
        }

        auto client = HttpClient::MakeSSL(host, port, kApiRequestNewDevice);
        auto resp = client->Post({
#ifdef WIN32
            {"platform", "windows"},
#else
            {"platform", "android"},
#endif
             {"hw_info", hw_info},
             {"appkey", appkey}
        });

        LOGI("RequestNewDevice, hw_info: {}, appkey: {}", hw_info, appkey);
        LOGI("NewDeviceResp, status: {}, body: {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("Request new device failed, code: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        if (auto r = ParseJsonAsDevice(resp.body); r) {
            return r;
        }
        else {
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    Result<SpvrDevicePtr, SpvrApiError> SpvrDeviceApi::UpdateRandomPwd(const std::string& host,
                                                                 int port,
                                                                 const std::string& appkey,
                                                                 const std::string& target_device_id) {
        auto client = HttpClient::MakeSSL(host, port, kApiUpdateRandomPwd);
        auto resp = client->Post({
            {"device_id", target_device_id},
            {"appkey", appkey}
        });

        LOGI("UpdateRandomPwd, status:{}, : {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("UpdateRandomPwd failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        if (auto r = ParseJsonAsDevice(resp.body); r) {
            return r;
        }
        else {
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    Result<SpvrDevicePtr, SpvrApiError> SpvrDeviceApi::UpdateSafetyPwd(const std::string& host,
                                                                 int port,
                                                                 const std::string& appkey,
                                                                 const std::string& target_device_id,
                                                                 const std::string& safety_pwd_md5) {
        auto client = HttpClient::MakeSSL(host, port, kApiUpdateSafetyPwd, 2000);
        auto resp = client->Post({
            {"device_id", target_device_id},
            {"safety_pwd_md5", safety_pwd_md5},
            {"appkey", appkey}
        });

        LOGI("UpdateSafetyPwd, status: {}, : {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("UpdateSafetyPwd failed: {}", resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        if (auto r = ParseJsonAsDevice(resp.body); r) {
            return r;
        }
        else {
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    Result<SpvrDevicePtr, SpvrApiError> SpvrDeviceApi::QueryDevice(const std::string& host,
                                                             int port,
                                                             const std::string& appkey,
                                                             const std::string& device_id) {
        auto client = HttpClient::MakeSSL(host, port, kApiQueryDeviceById);
        auto resp = client->Request({
            {"device_id", device_id},
            {"appkey", appkey}
        });

        //LOGI("Req path: {}", client->GetReqPath());
        //LOGI("QueryDevice, status: {}, : {}", resp.status, resp.body);
        if (resp.status != 200 || resp.body.empty()) {
            LOGE("GetDevice failed: {}, code: {}", device_id, resp.status);
            return TcErr((SpvrApiError)resp.status);
        }

        if (auto r = ParseJsonAsDevice(resp.body); r) {
            return r;
        }
        else {
            return TcErr(SpvrApiError::kParseJsonFailed);
        }
    }

    std::shared_ptr<SpvrDevice> SpvrDeviceApi::ParseJsonAsDevice(const std::string& body) {
        try {
            auto obj = json::parse(body);
            auto resp_device_id = obj["data"]["device_id"].get<std::string>();
            auto random_pwd_md5 = obj["data"]["random_pwd_md5"].get<std::string>();
            auto gen_random_pwd = obj["data"]["gen_random_pwd"].get<std::string>();
            auto safety_pwd_md5 = obj["data"]["safety_pwd_md5"].get<std::string>();
            auto used_time = obj["data"]["used_time"].get<int64_t>();
            auto created_timestamp = obj["data"]["created_timestamp"].get<int64_t>();
            auto last_update_timestamp = obj["data"]["last_update_timestamp"].get<int64_t>();
            //LOGI("PaserJsonAsDevice: {} => RPWD: {}, SPWD: {}", resp_device_id, random_pwd_md5, safety_pwd_md5);

            auto device = std::make_shared<SpvrDevice>();
            device->device_id_ = resp_device_id;
            device->gen_random_pwd_ = gen_random_pwd;
            device->random_pwd_md5_ = random_pwd_md5;
            device->safety_pwd_md5_ = safety_pwd_md5;
            device->used_time_ = used_time;
            device->created_timestamp_ = created_timestamp;
            device->updated_timestamp_ = last_update_timestamp;
            return device;
        } catch(std::exception& e) {
            LOGE("ParseJsonAsDevice failed: {}, message: {}", e.what(), body);
            return nullptr;
        }
    }
    
}
