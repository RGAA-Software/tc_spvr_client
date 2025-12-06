//
// Created by RGAA on 27/03/2025.
//

#ifndef GAMMARAY_SPVR_ERRORS_H
#define GAMMARAY_SPVR_ERRORS_H

namespace spvr
{

    enum class SpvrApiError {
        kInvalidHostAddress = 1,
        kParseJsonFailed,

        kInvalidParams = 600,
        kDatabaseError = 601,
        kDeviceNotFound = 602,
        kPasswordInvalid = 603,
        kInvalidAppkey = 604,
        kCreateDeviceFailed = 605,
        kInvalidAuthorization = 606,
        kInternalError = 607,
        kUserAlreadyExists = 608,
        kUserNotFound = 609,
        kUserUpdateFailed = 610,
        kFileNoExtension = 611,
        kUploadFileFailed = 612,
        kVerifyPasswordFailed = 613,
        kStreamNotFound = 614,
        kConnectionNotFound = 615,
        kUserDeviceNotFound = 616,
        kUserDeviceAlreadyExists = 617,
        kNeedDescParam = 618,
        kNeedVersionParam = 619,
        kVersionNotFound = 620,
        kFileNotFound = 621,
    };

    static std::string SpvrApiErrorAsString(const SpvrApiError& err) {
        return "";
    }

}

#endif //GAMMARAY_SPVR_ERRORS_H
