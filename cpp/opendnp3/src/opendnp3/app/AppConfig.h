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
#ifndef __APP_CONFIG_H_
#define __APP_CONFIG_H_

#include "opendnp3/DNPConstants.h"

using namespace openpal;

namespace opendnp3
{

/** Configuration structure for the dnp3 application layer
*/
struct AppConfig
{

	/// Default constructor
	AppConfig(bool aIsMaster = true) : IsMaster(aIsMaster), RspTimeout(TimeDuration::Seconds(5)), NumRetry(0), FragSize(DEFAULT_FRAG_SIZE) {}

	AppConfig(bool aIsMaster, TimeDuration aRspTimeout, size_t aNumRetry = 0, size_t aFragSize = DEFAULT_FRAG_SIZE) :
		IsMaster(aIsMaster),
		RspTimeout(aRspTimeout),
		NumRetry(aNumRetry),
		FragSize(aFragSize)
	{}

	/// True if the application layer parser should parse response types. Set to false for outstation
	bool IsMaster;

	/// The response/confirm timeout in millisec
	TimeDuration RspTimeout;

	/// Number of retries performed for applicable frames
	size_t NumRetry;

	/// The maximum size of received application layer fragments
	size_t FragSize;

};

}

#endif
