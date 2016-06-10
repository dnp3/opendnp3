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

#ifndef ASIODNP3_MANAGER_IMPL_H
#define ASIODNP3_MANAGER_IMPL_H

#include <openpal/logging/ILogHandler.h>

#include <openpal/util/Uncopyable.h>

#include <asiopal/ThreadPool.h>

#include <opendnp3/LogLevels.h>

#include "asiodnp3/ChannelSet.h"

namespace asiodnp3
{

class ManagerImpl : private openpal::Uncopyable
{

public:

	ManagerImpl(
	    uint32_t concurrencyHint,
	    std::shared_ptr<openpal::ILogHandler> handler,
	    std::function<void()> onThreadStart,
	    std::function<void()> onThreadExit
	) :
		handler(handler),
		threadpool(handler.get(), opendnp3::flags::INFO, concurrencyHint, onThreadStart, onThreadExit),
		channels()
	{}

	std::shared_ptr<openpal::ILogHandler> handler;
	asiopal::ThreadPool threadpool;
	ChannelSet channels;
};

}

#endif
