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

#include "asiodnp3/GPRSManagerImpl.h"

#include "asiodnp3/MasterTCPServer.h"

#include <opendnp3/LogLevels.h>

namespace asiodnp3
{

GPRSManagerImpl::~GPRSManagerImpl()
{
	this->BeginShutdown();
}

std::shared_ptr<asiopal::IListener> GPRSManagerImpl::CreateListener(
	std::string loggerid,
	openpal::LogFilters loglevel,
	asiopal::IPEndpoint endpoint,
	std::error_code& ec)
{
	std::lock_guard <std::mutex> lock(m_mutex);

	auto server = asiodnp3::MasterTCPServer::Create(m_pool.GetIOService(), m_log_root.GetLogger(), endpoint, ec);

	if (ec)
	{
		return nullptr;
	}
	else
	{
		this->m_servers.push_back(server);
		return server;
	}	
}

void GPRSManagerImpl::BeginShutdown()
{
	std::lock_guard <std::mutex> lock(m_mutex);

	this->m_is_shutting_down = true;

	for (auto& server : m_servers)
	{
		server->BeginShutdown();
	}

	m_servers.clear();
}

GPRSManagerImpl::GPRSManagerImpl(uint32_t concurrencyHint, openpal::ILogHandler& handler) :
	m_mutex(),
	m_log_handler(&handler),
	m_log_root(&handler, "manager", opendnp3::levels::NORMAL),
	m_is_shutting_down(false),
	m_log(&handler),
	m_pool(&handler, concurrencyHint, opendnp3::flags::INFO, [](){}, [](){})
{}

}


