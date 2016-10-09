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
#ifndef ASIODNP3_GPRSMANAGERIMPL_H
#define ASIODNP3_GPRSMANAGERIMPL_H

#include <asiopal/IListener.h>
#include <asiopal/ThreadPool.h>
#include <asiopal/IPEndpoint.h>
#include <asiopal/ResourceManagerBase.h>

#include <openpal/util/Uncopyable.h>
#include <openpal/logging/Logger.h>

#include <asiodnp3/IListenCallbacks.h>

#include <asiopal/TLSConfig.h>

#include <cstdint>
#include <memory>
#include <mutex>
#include <vector>

namespace asiodnp3
{

class GPRSManagerImpl final :
	private openpal::Uncopyable,
	private asiopal::ResourceManagerBase
{

public:

	GPRSManagerImpl(uint32_t concurrencyHint, std::shared_ptr<openpal::ILogHandler> handler);

	~GPRSManagerImpl();

	void BeginShutdown();

	std::shared_ptr<asiopal::IListener> CreateListener(
	    std::string loggerid,
	    openpal::LogFilters loglevel,
	    asiopal::IPEndpoint endpoint,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::error_code& ec
	);

	std::shared_ptr<asiopal::IListener> CreateListener(
	    std::string loggerid,
	    openpal::LogFilters loglevel,
	    asiopal::IPEndpoint endpoint,
	    const asiopal::TLSConfig& config,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::error_code& ec
	);

private:

	openpal::Logger logger;

	/// this will be the first thing to be destroyed forcing all handlers to be run
	std::shared_ptr<asiopal::ThreadPool> pool;

};

}

#endif
