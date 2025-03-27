//
// Created by RGAA on 27/03/2025.
//

#ifndef GAMMARAY_SPVR_ERRORS_H
#define GAMMARAY_SPVR_ERRORS_H

namespace tc
{

    enum class SpvrError {
        kSpvrOk,
        kSpvrUnknown,
        kSpvrParseJsonFailed,   // parse json failed
        kSpvrRequestFailed,     // http request failed
        kSpvrJustCodeError,     // code != 200
        // detail code errors below
        // kSpvr xxx
        kSpvrDataError,         // data is invalid
        kSpvrNoOnlineServers,   // don't have online servers
    };

    static std::string SpvrError2String(const SpvrError& err) {
        if (err == SpvrError::kSpvrOk) {
            return "Ok";
        }
        else if (err == SpvrError::kSpvrUnknown) {
            return "Unknown";
        }
        else if (err == SpvrError::kSpvrParseJsonFailed) {
            return "Parse json failed";
        }
        else if (err == SpvrError::kSpvrRequestFailed) {
            return "Network request failed";
        }
        else if (err == SpvrError::kSpvrJustCodeError) {
            return "Code error, != 200";
        }
        else if (err == SpvrError::kSpvrDataError) {
            return "Data error";
        }
        else if (err == SpvrError::kSpvrNoOnlineServers) {
            return "No online servers";
        }
        else {
            return "Unknown";
        }
    }

}

#endif //GAMMARAY_SPVR_ERRORS_H
