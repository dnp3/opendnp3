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

#ifndef __OPENDNP3_CONFIGURE_H_
#define __OPENDNP3_CONFIGURE_H_

#include <cstdint>

namespace opendnp3
{
namespace sizes
{

// the default size for an APDU
static const uint32_t DEFAULT_MAX_APDU_SIZE = 2048;

// the maximum number of static read object/variation records that can be in any READ request
static const uint16_t MAX_READ_REQUESTS = 8;

// a safeguard parameter to project from DoS attacks, this really only applies to "empty strings"
static const uint32_t MAX_OBJECTS_PER_APDU = 32768;

// the maximum number of queued commands for a master
static const uint8_t MAX_MASTER_POLL_TASKS = 8;

// the maximum number of queued users tasks like commands
static const uint8_t MAX_MASTER_USERS_TASKS = 8;

/// The miniumum allowed size for rx and tx APDUs
static const uint32_t MIN_APDU_SIZE = 20;

static_assert(MAX_MASTER_USERS_TASKS >= 8, "Max master user tasks must be at least 8");

}
}

#endif
