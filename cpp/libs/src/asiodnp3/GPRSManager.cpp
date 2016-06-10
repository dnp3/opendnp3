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

#include "asiodnp3/GPRSManager.h"

#include "asiodnp3/GPRSManagerImpl.h"

using namespace openpal;

namespace asiodnp3
{

GPRSManager::GPRSManager(uint32_t concurrencyHint, std::shared_ptr<openpal::ILogHandler> handler) :
	m_impl(new GPRSManagerImpl(concurrencyHint, handler))
{}


GPRSManager::~GPRSManager()
{
	delete m_impl;
}

void GPRSManager::BeginShutdown()
{
	m_impl->BeginShutdown();
}

std::shared_ptr<asiopal::IListener> GPRSManager::CreateListener(
    std::string loggerid,
    openpal::LogFilters loglevel,
    asiopal::IPEndpoint endpoint,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec
)
{
	return m_impl->CreateListener(loggerid, loglevel, endpoint, callbacks, ec);
}

#ifdef OPENDNP3_USE_TLS

std::shared_ptr<asiopal::IListener> GPRSManager::CreateListener(
    std::string loggerid,
    openpal::LogFilters loglevel,
    asiopal::IPEndpoint endpoint,
    const asiopal::TLSConfig& config,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec
)
{
	return m_impl->CreateListener(loggerid, loglevel, endpoint, config, callbacks, ec);
}

#endif

}




