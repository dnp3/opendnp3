/**
* Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
* more contributor license agreements. See the NOTICE file distributed
* with this work for additional information regarding copyright ownership.
* Green Energy Corp licenses this file to you under the Apache License,
* Version 2.0 (the "License"); you may not use this file except in
* compliance with the License.  You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* This project was forked on 01/01/2013 by Automatak, LLC and modifications
* may have been made to this file. Automatak, LLC licenses these modifications
* to you under the terms of the License.
*/

#ifndef OPENDNP3_LOGLEVELS_H
#define OPENDNP3_LOGLEVELS_H

#include <openpal/logging/LogLevels.h>

namespace opendnp3
{

namespace flags
{

// define most of these in terms of the base openpal filters
const int32_t EVENT = openpal::logflags::EVENT;
const int32_t ERR = openpal::logflags::ERR;
const int32_t WARN = openpal::logflags::WARN;
const int32_t INFO = openpal::logflags::INFO;
const int32_t DBG = openpal::logflags::DBG;

// upshift the custom dnp3 filters
const int32_t LINK_RX = DBG << 1;
const int32_t LINK_RX_HEX = LINK_RX << 1;

const int32_t LINK_TX = LINK_RX_HEX << 1;
const int32_t LINK_TX_HEX = LINK_TX << 1;

const int32_t TRANSPORT_RX = LINK_TX_HEX << 1;
const int32_t TRANSPORT_TX = TRANSPORT_RX << 1;

const int32_t APP_HEADER_RX = TRANSPORT_TX << 1;
const int32_t APP_HEADER_TX = APP_HEADER_RX << 1;

const int32_t APP_OBJECT_RX = APP_HEADER_TX << 1;
const int32_t APP_OBJECT_TX = APP_OBJECT_RX << 1;

}

namespace levels
{

const int32_t NOTHING = 0;
const int32_t ALL = ~NOTHING;
const uint32_t NORMAL = flags::EVENT | flags::ERR | flags::WARN | flags::INFO;
const uint32_t ALL_COMMS = flags::LINK_RX | flags::LINK_TX | flags::TRANSPORT_RX | flags::TRANSPORT_TX | flags::APP_HEADER_RX | flags::APP_HEADER_TX | flags::APP_OBJECT_RX | flags::APP_OBJECT_TX;

}

}

#endif
