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
#ifndef __STATIC_SIZE_CONFIGURATION_H_
#define __STATIC_SIZE_CONFIGURATION_H_

#include <cstdint>

// Default configurations for statically allocated buffers int the stack
// They are liberally set by default, but can be reduced for embedded systems

namespace opendnp3
{
namespace sizes
{
// the maximum number of static read object/variation records that can be in any READ request
static const uint16_t MAX_READ_REQUESTS = 16;

// a safeguard parameter to project from DoS attacks
// this really only applies to "empty strings"
static const uint32_t MAX_OBJECTS_PER_APDU = 32768;

// the maximum number of event read object/variation records that can be in any READ request
static const uint16_t MAX_EVENT_READ_REQUESTS = 16;

// the maximum number of queued commands for a master
static const uint8_t MAX_COMMAND_QUEUE_SIZE = 16;

// the maximum size of a transmitted APDU
static const uint32_t MAX_TX_APDU_SIZE = 2048;
static_assert(MAX_TX_APDU_SIZE >= 100, "APDU tx buffer size must be at least 100");

// the maximum size of a received APDU
static const uint32_t MAX_RX_APDU_SIZE = 2048;
static_assert(MAX_RX_APDU_SIZE >= 249, "APDU rx buffer size must be at least 249 bytes");

// the default APDU buffer size
static const uint32_t DEFAULT_APDU_BUFFER_SIZE = MAX_TX_APDU_SIZE;
static_assert(DEFAULT_APDU_BUFFER_SIZE <= MAX_TX_APDU_SIZE, "default buffer size must be less than the maximum");

// The maximum number of stacks that can associate to a link layer router
static const uint16_t MAX_STACKS_PER_CHANNEL = 16;
static_assert(MAX_STACKS_PER_CHANNEL > 0, "At least 1 stack is required per router");

// The number of bytes needed to hold an APDU fully encapsulated with TPDU/LPDU
static const uint32_t APDU_LPDU_BUFFER_SIZE = (((MAX_TX_APDU_SIZE / 249) + 1) * 292);

// default sizing is big enough to receive a full APDU with full LPDU's
static const uint32_t LINK_RECEIVER_BUFFER_SIZE = APDU_LPDU_BUFFER_SIZE;
static_assert(LINK_RECEIVER_BUFFER_SIZE >= 292, "Receiver must buffer at least 292 bytes");
}
}

#endif
