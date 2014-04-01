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

#ifndef __OPENDNP3_LOG_LEVELS_H_
#define __OPENDNP3_LOG_LEVELS_H_

#include <openpal/LogLevels.h>

namespace opendnp3
{
namespace flags
{
// define most of these in terms of the base openpal filters
const int32_t EVENT = openpal::log::EVENT;
const int32_t ERR = openpal::log::ERR;
const int32_t WARN = openpal::log::WARN;
const int32_t INFO = openpal::log::INFO;
const int32_t DEBUG = openpal::log::DEBUG;

// upshift the custom dnp3 filters
const int32_t INTERPRET = DEBUG << 1;
const int32_t COMM = INTERPRET << 1;
}

namespace levels
{
const int32_t NOTHING = 0;
const int32_t ALL = ~NOTHING;
const uint32_t NORMAL = flags::EVENT | flags::ERR | flags::WARN | flags::INFO;
}
}

#endif
