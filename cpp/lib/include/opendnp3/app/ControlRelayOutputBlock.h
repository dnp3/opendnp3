/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef OPENDNP3_CONTROLRELAYOUTPUTBLOCK_H
#define OPENDNP3_CONTROLRELAYOUTPUTBLOCK_H

#include "opendnp3/gen/CommandStatus.h"
#include "opendnp3/gen/OperationType.h"
#include "opendnp3/gen/TripCloseCode.h"

namespace opendnp3
{

/**
 * Describes an incoming control request from the master. It is the
 * applications responsibility to handle the request and return an
 * approiate status code. The PULSE_CLOSE and PULSE_TRIP OperationType
 * require setting the mOnTimeMS, mOffTimeMS and mCount variables, otherwise
 * just use the defaults.
 */
class ControlRelayOutputBlock
{
public:
    // primary constructor where the control code is split by its components
    ControlRelayOutputBlock(OperationType opType = OperationType::LATCH_ON,
                            TripCloseCode tcc = TripCloseCode::NUL,
                            bool clear = false,
                            uint8_t count = 1,
                            uint32_t onTime = 100,
                            uint32_t offTime = 100,
                            CommandStatus status = CommandStatus::SUCCESS);

    // overloaded constructor that allows the user to set a raw control code for non-standard codes
    ControlRelayOutputBlock(uint8_t rawCode,
                            uint8_t count = 1,
                            uint32_t onTime = 100,
                            uint32_t offTime = 100,
                            CommandStatus status = CommandStatus::SUCCESS);

    /// operation type
    OperationType opType;
    // trip-close code
    TripCloseCode tcc;
    // clear bit
    bool clear;
    /// the number of times to repeat the operation
    uint8_t count;
    /// the 'on' time for the pulse train
    uint32_t onTimeMS;
    /// the 'off' time for the pulse train
    uint32_t offTimeMS;
    /// status of the resulting operation
    CommandStatus status;
    /// The raw code in bytes
    uint8_t rawCode;

    bool IsQUFlagSet() const
    {
        return (rawCode & 0x10) != 0;
    }

    bool ValuesEqual(const ControlRelayOutputBlock& lhs) const
    {
        return (opType == lhs.opType) && (tcc == lhs.tcc) && (clear == lhs.clear) && (count == lhs.count)
            && (onTimeMS == lhs.onTimeMS) && (offTimeMS == lhs.offTimeMS);
    }

    bool operator==(const ControlRelayOutputBlock& lhs) const
    {
        return this->ValuesEqual(lhs) && (this->status == lhs.status);
    }
};

} // namespace opendnp3

#endif
