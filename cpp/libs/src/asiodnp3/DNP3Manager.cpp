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
#include "asiodnp3/DNP3Manager.h"


#include <opendnp3/LogLevels.h>

#include <asiopal/PhysicalLayerSerial.h>
#include <asiopal/PhysicalLayerTCPClient.h>
#include <asiopal/PhysicalLayerTCPServer.h>

#ifdef OPENDNP3_USE_TLS
#include <asiopal/tls/PhysicalLayerTLSClient.h>
#include <asiopal/tls/PhysicalLayerTLSServer.h>
#endif

#include "asiodnp3/ManagerImpl.h"

using namespace openpal;

namespace asiodnp3
{


DNP3Manager::DNP3Manager(
    uint32_t concurrencyHint,
    std::shared_ptr<openpal::ILogHandler> handler,
    std::function<void()> onThreadStart,
    std::function<void()> onThreadExit) :
	impl(new ManagerImpl(concurrencyHint, handler, onThreadStart, onThreadExit))
{

}

// this has to be here b/c of forward declared ManagerImpl
DNP3Manager::~DNP3Manager()
{

}

void DNP3Manager::Shutdown()
{
	impl->channels.Shutdown();
}

IChannel* DNP3Manager::AddTCPClient(
    char const* id,
    uint32_t levels,
    const opendnp3::ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port)
{
	auto root = std::unique_ptr<LogRoot>(new LogRoot(impl->handler.get(), id, levels));
	auto phys = std::unique_ptr<asiopal::PhysicalLayerTCPClient>(new asiopal::PhysicalLayerTCPClient(root->logger, impl->threadpool.GetIOService(), host, local, port));
	return impl->channels.CreateChannel(std::move(root), retry, std::move(phys));
}

IChannel* DNP3Manager::AddTCPServer(
    char const* id,
    uint32_t levels,
    const opendnp3::ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port)
{

	auto root = std::unique_ptr<LogRoot>(new LogRoot(impl->handler.get(), id, levels));
	auto phys = std::unique_ptr<asiopal::PhysicalLayerTCPServer>(
	                new asiopal::PhysicalLayerTCPServer(root->logger, impl->threadpool.GetIOService(), endpoint, port)
	            );
	return impl->channels.CreateChannel(std::move(root), retry, std::move(phys));
}

IChannel* DNP3Manager::AddSerial(
    char const* id,
    uint32_t levels,
    const opendnp3::ChannelRetry& retry,
    asiopal::SerialSettings settings)
{

	auto root = std::unique_ptr<LogRoot>(new LogRoot(impl->handler.get(), id, levels));
	auto phys = std::unique_ptr<asiopal::PhysicalLayerSerial>(
	                new asiopal::PhysicalLayerSerial(root->logger, impl->threadpool.GetIOService(), settings)
	            );
	return impl->channels.CreateChannel(std::move(root), retry, std::move(phys));
}

#ifdef OPENDNP3_USE_TLS

IChannel* DNP3Manager::AddTLSClient(
    char const* id,
    uint32_t levels,
    const opendnp3::ChannelRetry& retry,
    const std::string& host,
    const std::string& local,
    uint16_t port,
    const asiopal::TLSConfig& config,
    std::error_code& ec)
{
	auto root = std::unique_ptr<LogRoot>(new LogRoot(impl->handler.get(), id, levels));
	auto phys = std::unique_ptr<asiopal::PhysicalLayerTLSClient>(
	                new asiopal::PhysicalLayerTLSClient(root->logger, impl->threadpool.GetIOService(), host, local, port, config, ec)
	            );
	return ec ? nullptr : impl->channels.CreateChannel(std::move(root), retry, std::move(phys));
}

IChannel* DNP3Manager::AddTLSServer(
    char const* id,
    uint32_t levels,
    const opendnp3::ChannelRetry& retry,
    const std::string& endpoint,
    uint16_t port,
    const asiopal::TLSConfig& config,
    std::error_code& ec)
{
	auto root = std::unique_ptr<LogRoot>(new LogRoot(impl->handler.get(), id, levels));
	auto phys = std::unique_ptr<asiopal::PhysicalLayerTLSServer>(
	                new asiopal::PhysicalLayerTLSServer(root->logger, impl->threadpool.GetIOService(), endpoint, port, config, ec)
	            );
	return ec ? nullptr : impl->channels.CreateChannel(std::move(root), retry, std::move(phys));
}

#endif

}
