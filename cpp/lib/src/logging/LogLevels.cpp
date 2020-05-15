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

#include "opendnp3/logging/LogLevels.h"

namespace opendnp3
{

const char* LogFlagToString(const LogLevel& flag)
{
    if (flag == flags::EVENT)
        return "EVENT  ";
    else if (flag == flags::ERR)
        return "ERROR  ";
    else if (flag == flags::WARN)
        return "WARN   ";
    else if (flag == flags::INFO)
        return "INFO   ";
    else if (flag == flags::DBG)
        return "DEBUG  ";
    else if (flag == flags::LINK_RX || flag == flags::LINK_RX_HEX)
        return "<-LL-- ";
    else if (flag == flags::LINK_TX || flag == flags::LINK_TX_HEX)
        return "--LL-> ";
    else if (flag == flags::TRANSPORT_RX)
        return "<-TL-- ";
    else if (flag == flags::TRANSPORT_TX)
        return "--TL-> ";
    else if (flag == flags::APP_HEADER_RX || flag == flags::APP_OBJECT_RX || flag == flags::APP_HEX_RX)
        return "<-AL-- ";
    else if (flag == flags::APP_HEADER_TX || flag == flags::APP_OBJECT_TX || flag == flags::APP_HEX_TX)
        return "--AL-> ";
    return "UNKNOWN";
}

} // namespace opendnp3
