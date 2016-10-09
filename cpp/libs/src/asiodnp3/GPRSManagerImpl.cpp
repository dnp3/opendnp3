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
    openpal::LogFilters levels,
    asiopal::IPEndpoint endpoint,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec)
{
	auto create = [&]() -> std::shared_ptr<asiopal::IListener>
	{
		auto newlogger = this->logger.Detach(loggerid, levels);

		auto handler = asiodnp3::MasterTCPServerHandler::Create(
			newlogger,
		    callbacks,
		    *this
		);

		return asiopal::TCPServer::Create(
		    asiopal::StrandExecutor::Create(this->pool),
		    handler,
			newlogger,
		    endpoint,
		    ec
		);
	};

	auto listener = this->BindResource<asiopal::IListener>(create);

	if (!listener)
	{
		ec = Error::SHUTTING_DOWN;
	}

	return listener;
}

std::shared_ptr<asiopal::IListener> GPRSManagerImpl::CreateListener(
    std::string loggerid,
    openpal::LogFilters levels,
    asiopal::IPEndpoint endpoint,
    const asiopal::TLSConfig& config,
    std::shared_ptr<IListenCallbacks> callbacks,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS

	auto create = [&]() -> std::shared_ptr<asiopal::IListener>
	{
		auto newlogger = this->logger.Detach(loggerid, levels);

		auto handler = asiodnp3::MasterTLSServerHandler::Create(
			newlogger,
		    callbacks,
		    *this
		);

		return asiopal::TLSServer::Create(
		    asiopal::StrandExecutor::Create(this->pool),
		    handler,
			newlogger,
		    endpoint,
		    config,
		    ec
		);

	};

	auto listener = this->BindResource<asiopal::IListener>(create);

	if (!listener)
	{
		ec = Error::SHUTTING_DOWN;
	}

	return listener;

#else

	ec = Error::NO_TLS_SUPPORT;
	return nullptr;

#endif

}


void GPRSManagerImpl::BeginShutdown()
{
	this->ShutdownResources();
}

GPRSManagerImpl::GPRSManagerImpl(uint32_t concurrencyHint, std::shared_ptr<openpal::ILogHandler> handler) :
	logger(handler, "manager", opendnp3::levels::NORMAL),
	pool(asiopal::ThreadPool::Create(logger, concurrencyHint, opendnp3::flags::INFO))
{}

}


