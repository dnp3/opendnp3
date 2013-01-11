/*
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership.  Green Enery
 * Corp licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 */

#ifndef __VTO_ROUTER_SETTINGS_H_
#define __VTO_ROUTER_SETTINGS_H_

#include <APL/Types.h>

namespace apl
{
namespace dnp
{

/**
 * Settings classes used to configure the router.
 */
struct VtoRouterSettings {
	VtoRouterSettings() {}

	/**
	 * @param aChannelId Each dnp index for Vto data events is a channel id
	 * @param aStartLocal If true we allways try to keep the local connection online, otherwise we only connect when the remote side connects
	 * @param aDisableExtensions If true, the router defaults to the VTO specification and does not publish/utilize the connection state information
	 * @param aOpenRetryMs how long to wait before retrying opening the physical layer after a failure
	 */
	VtoRouterSettings(uint8_t aChannelId, bool aStartLocal, bool aDisableExtensions, millis_t aOpenRetryMs = 5000, millis_t aMaxOpenRetry = 10000);

	uint8_t CHANNEL_ID;
	millis_t MIN_OPEN_RETRY_MS;
	millis_t MAX_OPEN_RETRY_MS;
	bool START_LOCAL;
	bool DISABLE_EXTENSIONS;
};

}
}

/* vim: set ts=4 sw=4: */

#endif

