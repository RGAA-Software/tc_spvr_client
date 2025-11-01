//
// Created by RGAA on 1/11/2025.
//

#include "spvr_stream.h"

namespace spvr
{

    bool SpvrStream::IsValid() const {
        return !stream_id_.empty();
    }

    bool SpvrStream::IsRelay() const {
        return network_type_ == "relay" || !remote_device_id_.empty();
    }

}