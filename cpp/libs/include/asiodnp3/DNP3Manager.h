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

#include <openpal/logging/ILogHandler.h>
#include <openpal/executor/TimeDuration.h>

#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/link/ChannelRetry.h>

#include <asiodnp3/IChannel.h>
#include <asiopal/SerialTypes.h>

#ifdef OPENDNP3_USE_TLS
#include <asiopal/tls/TLSConfig.h>
#endif

#include <memory>
#include <system_error>

namespace asiodnp3
{

class ManagerImpl;


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
	*	@param handler Callback interface for log messages
	*	@param onThreadStart Action to run when a thread pool thread starts
	*	@param onThreadExit Action to run just before a thread pool thread exits
	*/
	DNP3Manager(
	    uint32_t concurrencyHint,
	    std::shared_ptr<openpal::ILogHandler> handler = std::shared_ptr<openpal::ILogHandler>(),
	std::function<void()> onThreadStart = []() {},
	std::function<void()> onThreadExit = []() {}
	);

	~DNP3Manager();

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

	/**
	* Add a persistent TCP client channel. Automatically attempts to reconnect.
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param retry Retry parameters for failed channels
	* @param host IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
	* @param port Port of remote outstation is listening on
	* @return A channel interface
	*/
	IChannel* AddTCPClient(
	    char const* id,
	    uint32_t levels,
	    const opendnp3::ChannelRetry& retry,
	    const std::string& host,
	    const std::string& local,
	    uint16_t port);

	/**
	* Add a persistent TCP server channel. Only accepts a single connection at a time.
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param retry Retry parameters for failed channels
	* @param endpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param port Port to listen on
	* @return A channel interface
	*/
	IChannel* AddTCPServer(
	    char const* id,
	    uint32_t levels,
	    const opendnp3::ChannelRetry& retry,
	    const std::string& endpoint,
	    uint16_t port);

	/**
	* Add a persistent TCP serial channel
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param retry Retry parameters for failed channels
	* @param settings settings object that fully parameterizes the serial port
	* @return A channel interface
	*/
	IChannel* AddSerial(
	    char const* id,
	    uint32_t levels,
	    const opendnp3::ChannelRetry& retry,
	    asiopal::SerialSettings settings);


#ifdef OPENDNP3_USE_TLS

	/**
	* Add a TLS client channel
	*
	* @throw std::system_error Throws underlying ASIO exception of TLS configuration is invalid
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param retry Retry parameters for failed channels
	* @param host IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param local adapter address on which to attempt the connection (use 0.0.0.0 for all adapters)
	* @param port Port of remote outstation is listening on
	* @param config TLS configuration information
	* @param An error code. If set, a nullptr will be returned
	* @return A channel interface
	*/
	IChannel* AddTLSClient(
	    char const* id,
	    uint32_t levels,
	    const opendnp3::ChannelRetry& retry,
	    const std::string& host,
	    const std::string& local,
	    uint16_t port,
	    const asiopal::TLSConfig& config,
	    std::error_code& ec);


	/**
	* Add a TLS server channel
	*
	* @throw std::system_error Throws underlying ASIO exception of TLS configuration is invalid
	*
	* @param id Alias that will be used for logging purposes with this channel
	* @param levels Bitfield that describes the logging level for this channel and associated sessions
	* @param retry Retry parameters for failed channels
	* @param endpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param port Port to listen on
	* @param config TLS configuration information
	* @param An error code. If set, a nullptr will be returned
	* @return A channel interface
	*/
	IChannel* AddTLSServer(
	    char const* id,
	    uint32_t levels,
	    const opendnp3::ChannelRetry& retry,
	    const std::string& endpoint,
	    uint16_t port,
	    const asiopal::TLSConfig& config,
	    std::error_code& ec);

#endif

private:

	std::unique_ptr<ManagerImpl> impl;

};

}

#endif
