/**
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

#include <opendnp3/gen/ChannelState.h>
#include <opendnp3/link/IOpenDelayStrategy.h>

#include <asiodnp3/IChannel.h>
#include <asiodnp3/IMaster.h>
#include <asiodnp3/IOutstation.h>

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

class DNP3Manager
{

public:

	DNP3Manager(
	    uint32_t concurrencyHint,
		std::function<void()> onThreadStart = []() {},
		std::function<void()> onThreadExit = []() {}
	);	

	~DNP3Manager();

	/**
	* Add a callback to receive log messages
	* @param pHandler Pointer to a callback object
	*/
	void AddLogSubscriber(openpal::ILogHandler* pHandler);	

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

	/**
	* Add a tcp client channel
	*
	* @param logger Logger that will be used for all log messages
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param host IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param port Port of remote outstation is listening on
	* @param strategy Reconnection delay strategy, default to exponential
	*/
	IChannel* AddTCPClient(
		char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    const std::string& host,
	    uint16_t port,	    
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

	/**
	* Add a tcp server channel
	*
	* @param logger Logger that will be used for all log messages
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param endpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param port Port to listen on
	* @param strategy Reconnection delay strategy, default to exponential
	*/
	IChannel* AddTCPServer(
		char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    const std::string& endpoint,
	    uint16_t port,		
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

	/**
	* Add a serial channel
	*
	* @param logger Logger that will be used for all log messages
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param settings settings object that fully parameterizes the serial port
	* @param strategy Reconnection delay strategy, default to exponential
	*/
	IChannel* AddSerial(
		char const* id,
	    uint32_t levels,
	    openpal::TimeDuration minOpenRetry,
	    openpal::TimeDuration maxOpenRetry,
	    asiopal::SerialSettings settings,		
	    opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

private:

	std::unique_ptr<asiopal::LogFanoutHandler> pFanoutHandler;
	std::unique_ptr<asiopal::IOServiceThreadPool> pThreadPool;
	std::unique_ptr<ChannelSet> pChannelSet;
};

}

#endif
