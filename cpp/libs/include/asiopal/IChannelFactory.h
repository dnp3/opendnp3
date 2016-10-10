/*
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
#ifndef ASIOPAL_ICHANNELFACTORY_H
#define ASIOPAL_ICHANNELFACTORY_H

#include "asiopal/IAsyncChannel.h"

#include <memory>

namespace asiopal
{

class IChannelFactory
{

public:

	typedef std::function<void(const std::shared_ptr<IAsyncChannel>&)> channel_callback_t;

	// permanently shutdown the factory
	virtual void Shutdown() = 0;

	// Begin the process of acquiring channels
	virtual void BeginChannelAccept(const channel_callback_t& callback) = 0;

	// Stop the process of acquiring channels
	virtual void SuspendChannelAccept() = 0;

	// Called when a currently active channel shuts down unexpectedly. May cause the factory to acquire another channel
	virtual void OnChannelShutdown(const channel_callback_t& callback) = 0;


};

}

#endif
