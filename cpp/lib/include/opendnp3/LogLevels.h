/*
 * Copyright 2013-2019 Automatak, LLC
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

#ifndef OPENDNP3_LOGLEVELS_H
#define OPENDNP3_LOGLEVELS_H

#include <log4cpp/LogLevels.h>

namespace opendnp3
{

const char* LogFlagToString(const log4cpp::LogLevel& flag);

namespace flags
{

    // define most of these in terms of the base log4cpp filters
    const log4cpp::LogLevel EVENT = log4cpp::levels::event;
    const log4cpp::LogLevel ERR = log4cpp::levels::error;
    const log4cpp::LogLevel WARN = log4cpp::levels::warn;
    const log4cpp::LogLevel INFO = log4cpp::levels::info;
    const log4cpp::LogLevel DBG = log4cpp::levels::debug;

    // up-shift the custom dnp3 filters

    const log4cpp::LogLevel LINK_RX = DBG.next();
    const log4cpp::LogLevel LINK_RX_HEX = DBG.next();

    const log4cpp::LogLevel LINK_TX = DBG.next();
    const log4cpp::LogLevel LINK_TX_HEX = DBG.next();

    const log4cpp::LogLevel TRANSPORT_RX = DBG.next();
    const log4cpp::LogLevel TRANSPORT_TX = DBG.next();

    const log4cpp::LogLevel APP_HEADER_RX = DBG.next();
    const log4cpp::LogLevel APP_HEADER_TX = DBG.next();

    const log4cpp::LogLevel APP_OBJECT_RX = DBG.next();
    const log4cpp::LogLevel APP_OBJECT_TX = DBG.next();

    const log4cpp::LogLevel APP_HEX_RX = DBG.next();
    const log4cpp::LogLevel APP_HEX_TX = DBG.next();

} // namespace flags

namespace levels
{

    const log4cpp::LogLevels NOTHING = log4cpp::LogLevels::none();
    const log4cpp::LogLevels ALL = log4cpp::LogLevels::everything();
    const log4cpp::LogLevels NORMAL = NOTHING | flags::EVENT | flags::ERR | flags::WARN | flags::INFO;
    const log4cpp::LogLevels ALL_APP_COMMS = NOTHING | flags::APP_HEADER_RX | flags::APP_HEADER_TX | flags::APP_OBJECT_RX
        | flags::APP_OBJECT_TX | flags::APP_HEX_RX | flags::APP_HEX_TX;
    const log4cpp::LogLevels ALL_COMMS
        = ALL_APP_COMMS | flags::LINK_RX | flags::LINK_TX | flags::TRANSPORT_RX | flags::TRANSPORT_TX;

} // namespace levels

} // namespace opendnp3

#endif
