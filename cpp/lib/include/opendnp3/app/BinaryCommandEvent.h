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

#ifndef OPENDNP3_BINARYCOMMANDEVENT_H
#define OPENDNP3_BINARYCOMMANDEVENT_H

#include "opendnp3/app/DNPTime.h"
#include "opendnp3/app/Flags.h"
#include "opendnp3/gen/CommandStatus.h"

namespace opendnp3
{

/**
Maps to Group13Var1/2
*/
class BinaryCommandEvent
{
public:
    BinaryCommandEvent();

    BinaryCommandEvent(Flags flags);

    BinaryCommandEvent(Flags flags, DNPTime time);

    BinaryCommandEvent(bool value, CommandStatus status);

    BinaryCommandEvent(bool value, CommandStatus status, DNPTime time);

    bool value;
    CommandStatus status;
    DNPTime time;

    Flags GetFlags() const;

    bool operator==(const BinaryCommandEvent& rhs) const;

private:
    static const uint8_t ValueMask = 0x80;
    static const uint8_t StatusMask = 0x7F;

    static bool GetValueFromFlags(Flags flags);
    static CommandStatus GetStatusFromFlags(Flags flags);
};

} // namespace opendnp3

#endif
