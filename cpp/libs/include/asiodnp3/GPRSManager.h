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
#ifndef ASIODNP3_GPRSMANAGER_H
#define ASIODNP3_GPRSMANAGER_H

#include <asiopal/IListener.h>
#include <asiopal/IPEndpoint.h>
#include <openpal/logging/ILogHandler.h>
#include <asiodnp3/IListenCallbacks.h>

#ifdef OPENDNP3_USE_TLS
#include <asiopal/tls/TLSConfig.h>
#endif

#include <system_error>

namespace asiodnp3
{

class GPRSManagerImpl;

/**
*	Root DNP3 object used to create listeners
*/
class GPRSManager
{

public:

	/**
	*	Construct a manager
	*
	*	@param concurrencyHint How many threads to allocate in the thread pool
	*/
	GPRSManager(uint32_t concurrencyHint, std::shared_ptr<openpal::ILogHandler> handler);

	~GPRSManager();

	/**
	* Begin the shutdown process. Also called by the destructor automatically
	*/
	void BeginShutdown();

	/**
	* Create a TCP listener that will be used to accept incoming connections
	*/
	std::shared_ptr<asiopal::IListener> CreateListener(
	    std::string loggerid,
	    openpal::LogFilters loglevel,
	    asiopal::IPEndpoint endpoint,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::error_code& ec
	);

#ifdef OPENDNP3_USE_TLS

	/**
	* Create a TLS listener that will be used to accept incoming connections
	*/
	std::shared_ptr<asiopal::IListener> CreateListener(
	    std::string loggerid,
	    openpal::LogFilters loglevel,
	    asiopal::IPEndpoint endpoint,
	    const asiopal::TLSConfig& config,
	    std::shared_ptr<IListenCallbacks> callbacks,
	    std::error_code& ec
	);
#endif

private:

	GPRSManagerImpl* m_impl;

};

}

#endif
