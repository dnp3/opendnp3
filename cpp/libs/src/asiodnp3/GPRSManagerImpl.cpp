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

#include "asiodnp3/ErrorCodes.h"

#include "asiopal/TCPServer.h"
#include "asiodnp3/MasterTCPServerHandler.h"

#ifdef OPENDNP3_USE_TLS
#include "asiopal/tls/TLSServer.h"
#include "asiodnp3/tls/MasterTLSServerHandler.h"
#endif

#include "opendnp3/LogLevels.h"


namespace asiodnp3
{

GPRSManagerImpl::~GPRSManagerImpl()
{
	this->BeginShutdown();

	// block on the pool until it is gone
	this->pool->Shutdown();
}

std::shared_ptr<asiopal::IListener> GPRSManagerImpl::CreateListener(
    std::string loggerid,
    openpal::LogFilters loglevel,
    asiopal::IPEndpoint endpoint,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec)
{
	std::lock_guard <std::mutex> lock(this->mutex);

	if (this->is_shutting_down)
	{
		ec = make_error_code(Error::SHUTTING_DOWN);
		return nullptr;
	}

	auto handler = asiodnp3::MasterTCPServerHandler::Create(
		this->log_root.Clone(loggerid.c_str(), loglevel),
		callbacks,
		*this
	);

	auto server = asiopal::TCPServer::Create(				
		asiopal::StrandExecutor::Create(this->pool),
		handler,
		*this,
		this->log_root.Clone(loggerid.c_str(), loglevel),
		endpoint,
		ec
	);

	if (ec)
	{
		return nullptr;
	}

	this->resources.push_back(server);

	return server;
}

std::shared_ptr<asiopal::IListener> GPRSManagerImpl::CreateListener(
    std::string loggerid,
    openpal::LogFilters loglevel,
    asiopal::IPEndpoint endpoint,
    const asiopal::TLSConfig& config,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS

	std::lock_guard <std::mutex> lock(this->mutex);

	if (this->is_shutting_down)
	{
		ec = make_error_code(Error::SHUTTING_DOWN);
		return nullptr;
	}

	auto handler = asiodnp3::MasterTLSServerHandler::Create(
	                   this->log_root.Clone(loggerid.c_str(), loglevel),
	                   callbacks,
	                   *this
	               );

	auto server = asiopal::TLSServer::Create(
	                  asiopal::StrandExecutor::Create(this->pool),
	                  handler,
	                  *this,
	                  this->log_root.Clone(loggerid.c_str(), loglevel),
	                  endpoint,
	                  config,
	                  ec
	              );

	if (ec)
	{
		return nullptr;
	}

	this->resources.push_back(server);
	return server;

#else

	ec = Error::NO_TLS_SUPPORT;
	return nullptr;

#endif

}


void GPRSManagerImpl::BeginShutdown()
{
	std::lock_guard <std::mutex> lock(this->mutex);

	this->is_shutting_down = true;

	for (auto& resource : this->resources)
	{
		resource->BeginShutdown();
	}
}

GPRSManagerImpl::GPRSManagerImpl(uint32_t concurrencyHint, std::shared_ptr<openpal::ILogHandler> handler) :
	mutex(),
	log_handler(handler),
	log_root(handler.get(), "gprs-manager", opendnp3::levels::NORMAL),
	is_shutting_down(false),
	pool(asiopal::ThreadPool::Create(handler.get(), concurrencyHint, opendnp3::flags::INFO))
{}

bool GPRSManagerImpl::Register(std::shared_ptr<asiopal::IResource> resource)
{
	std::lock_guard <std::mutex> lock(this->mutex);
	if (this->is_shutting_down)
	{
		return false;
	}
	this->resources.push_back(resource);
	return true;
}

void GPRSManagerImpl::Unregister(std::shared_ptr<asiopal::IResource> resource)
{
	std::lock_guard <std::mutex> lock(this->mutex);

	auto is_match = [resource](const std::shared_ptr<asiopal::IResource>& other) -> bool
	{
		return resource == other;
	};

	this->resources.erase(std::remove_if(this->resources.begin(), this->resources.end(), is_match), this->resources.end());

}

}


