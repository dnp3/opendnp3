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
#ifndef ASIODNP3_DNP3MANAGER_H
#define ASIODNP3_DNP3MANAGER_H

#include <cstdint>
#include <functional>
#include <memory>

#include <asiopal/SerialTypes.h>

#include <openpal/logging/ILogHandler.h>
#include <openpal/executor/TimeDuration.h>
#include <openpal/crypto/ICryptoProvider.h>


#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/link/IOpenDelayStrategy.h>


#include <asiodnp3/IChannel.h>
#include <asiodnp3/IMaster.h>
#include <asiodnp3/IOutstation.h>

#ifdef OPENDNP3_USE_TLS
#include <asiotls/TLSConfig.h>
#endif

namespace asiopal
{
class EventLog;
class LogFanoutHandler;
class IOServiceThreadPool;
}

namespace asiodnp3
{

class DNP3Channel;
class ChannelSet;


/**
*	Root DNP3 object used to create channels and sessions.
*/
class DNP3Manager
{

public:

	/**
	*	Construct a manager
	*
	*	@param concurrencyHint How many threads to allocate in the thread pool
	*	@param crypto Optional cryptography interface for secure authentication
	*	@param onThreadStart Action to run when a thread pool thread starts
	*	@param onThreadExit Action to run just before a thread pool thread exits
	*/
	DNP3Manager(
	    uint32_t concurrencyHint,
	    openpal::ICryptoProvider* crypto = nullptr,
		std::function<void()> onThreadStart = []() {},
		std::function<void()> onThreadExit = []() {}
	);

	~DNP3Manager();

	/**
	* Add a callback to receive log messages
	* @param handler pointer to a log handling interface
	*/
	void AddLogSubscriber(openpal::ILogHandler* handler);

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

	/**
	* Add a tcp client channel
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param host IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
	* @param port Port of remote outstation is listening on
	* @param strategy Reconnection delay strategy, default to exponential backoff
	* @return A channel interface
	*/
	IChannel* AddTCPClient(
	    char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    const std::string& host,
	    const std::string& local,
	    uint16_t port,
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

	/**
	* Add a tcp server channel
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param endpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param port Port to listen on
	* @param strategy Reconnection delay strategy, default to exponential
	* @return A channel interface
	*/
	IChannel* AddTCPServer(
	    char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    const std::string& endpoint,
	    uint16_t port,		
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

#ifdef OPENDNP3_USE_TLS

	/**
	* Add a TLS client channel
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param host IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
	* @param port Port of remote outstation is listening on
	* @param config TLS configuration information	
	* @param strategy Reconnection delay strategy, default to exponential backoff
	* @return A channel interface
	*/
	IChannel* AddTLSClient(
		char const* id,
		uint32_t levels,
		openpal::TimeDuration minOpenRetry,
		openpal::TimeDuration maxOpenRetry,
		const std::string& host,
		const std::string& local,
		uint16_t port,
		const asiotls::TLSConfig& config,
		opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

#endif

	/**
	* Add a serial channel
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param settings settings object that fully parameterizes the serial port
	* @param strategy Reconnection delay strategy, default to exponential
	* @return A channel interface
	*/
	IChannel* AddSerial(
	    char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    asiopal::SerialSettings settings,
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

private:

	openpal::ICryptoProvider* pCrypto;
	std::unique_ptr<asiopal::LogFanoutHandler> pFanoutHandler;
	std::unique_ptr<asiopal::IOServiceThreadPool> pThreadPool;
	std::unique_ptr<ChannelSet> pChannelSet;

};

}

#endif
