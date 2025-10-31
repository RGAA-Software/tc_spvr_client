//
// Created by RGAA on 27/03/2025.
//

#ifndef GAMMARAY_SPVR_ERRORS_H
#define GAMMARAY_SPVR_ERRORS_H

namespace spvr
{

    enum class SpvrApiError {
        kInvalidHostAddress,
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
    };

}

#endif //GAMMARAY_SPVR_ERRORS_H
