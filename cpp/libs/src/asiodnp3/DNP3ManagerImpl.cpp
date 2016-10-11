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

#include "DNP3ManagerImpl.h"

#include <opendnp3/LogLevels.h>


#ifdef OPENDNP3_USE_TLS

#endif

#include "asiodnp3/ErrorCodes.h"

using namespace openpal;

namespace asiodnp3
{

DNP3ManagerImpl::DNP3ManagerImpl(
    uint32_t concurrencyHint,
    std::shared_ptr<openpal::ILogHandler> handler,
    std::function<void()> onThreadStart,
    std::function<void()> onThreadExit
) :
	logger(handler, "manager", opendnp3::levels::ALL),
	threadpool(logger, opendnp3::flags::INFO, concurrencyHint, onThreadStart, onThreadExit),
	resources(asiopal::ResourceManager::Create())
{}

void DNP3ManagerImpl::Shutdown()
{
	if (resources)
	{
		resources->Shutdown();
		resources.reset();
	}
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTCPClient(
    const std::string& id,
    uint32_t levels,
    const asiopal::ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    std::shared_ptr<IChannelListener> listener)
{
	auto clogger = this->logger.Detach(id, levels);
	return nullptr;
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTCPServer(
    const std::string& id,
    uint32_t levels,
    const asiopal::ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port,
    std::shared_ptr<IChannelListener> listener)
{
	auto clogger = this->logger.Detach(id, levels);
	return nullptr;
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddSerial(
    const std::string& id,
    uint32_t levels,
    const asiopal::ChannelRetry& retry,
    asiopal::SerialSettings settings,
    std::shared_ptr<IChannelListener> listener)
{
	//ec = Error::NO_SERIAL_SUPPORT;
	return nullptr;
}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTLSClient(
    const std::string& id,
    uint32_t levels,
    const asiopal::ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    const asiopal::TLSConfig& config,
    std::shared_ptr<IChannelListener> listener,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#else
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#endif

}

std::shared_ptr<IChannel> DNP3ManagerImpl::AddTLSServer(
    const std::string& id,
    uint32_t levels,
    const asiopal::ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port,
    const asiopal::TLSConfig& config,
    std::shared_ptr<IChannelListener> listener,
    std::error_code& ec)
{

#ifdef OPENDNP3_USE_TLS
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#else
	ec = Error::NO_TLS_SUPPORT;
	return nullptr;
#endif

}

}

