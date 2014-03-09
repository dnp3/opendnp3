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

#include "Uncopyable.h"

#include <cstdint>

// Default configurations for statically allocated buffers int the stack
// They are liberally set by default, but can be reduced for embedded systems

namespace opendnp3
{

struct SizeConfiguration : private PureStatic
{
	// the maximum number of event buffers that can be bound to a database
	static const uint16_t MAX_EVENT_BUFFERS = 10;

	// the maximum number of static read object/variation records that can be in any READ request
	static const uint16_t MAX_READ_REQUESTS = 16;

	// the maximum number of event read object/variation records that can be in any READ request
	static const uint16_t MAX_EVENT_READ_REQUESTS = 16;

	// the maximum size of a transmitted or received APDU
	static const uint32_t MAX_APDU_BUFFER_SIZE = 2048;

	// a safeguard parameter to project from DoS attacks
	// this really only applies to "empty strings"
	static const uint32_t MAX_OBJECTS_PER_APDU = 32768;
};

}

#endif
