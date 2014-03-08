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
#ifndef __ASIO_DNP3_MANAGER_H_
#define __ASIO_DNP3_MANAGER_H_

#include <cstdint>
#include <functional>
#include <memory>

#include <openpal/Logger.h>
#include <openpal/TimeDuration.h>
#include <asiopal/SerialTypes.h>
#include <opendnp3/DestructorHook.h>
#include <opendnp3/link/IOpenDelayStrategy.h>

namespace opendnp3
{
class IChannel;
class DNP3Channel;
class DNP3Manager;
}

namespace openpal
{
class IPhysicalLayerAsync;
}

namespace asiopal
{
class EventLog;
class IOServiceThreadPool;
}


namespace asiodnp3
{

class ASIODNP3Manager : public opendnp3::DestructorHook
{
public:
	ASIODNP3Manager(
	        uint32_t aConcurrency,
	std::function<void()> aOnThreadStart = []() {},
	std::function<void()> aOnThreadExit = []() {}
	);

	~ASIODNP3Manager();

	/**
	* Add a callback to receive log messages
	* @param apLog Pointer to a callback object
	*/
	void AddLogSubscriber(openpal::ILogBase* apLog);

	openpal::ILogBase* GetLog();

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
	* @param pStrategy Reconnection delay strategy, default to exponential
	*/
	opendnp3::IChannel* AddTCPClient(
		openpal::Logger logger, 
		openpal::TimeDuration minOpenRetry, 
		openpal::TimeDuration maxOpenRetry, 
		const std::string& host, 
		uint16_t port,
		opendnp3::IOpenDelayStrategy* pStrategy = opendnp3::ExponentialBackoffStrategy::Inst());

	/**
	* Add a tcp server channel
	*
	* @param logger Logger that will be used for all log messages	
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param endpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param port Port to listen on
	* @param pStrategy Reconnection delay strategy, default to exponential
	*/
	opendnp3::IChannel* AddTCPServer(
		openpal::Logger,
		openpal::TimeDuration minOpenRetry,
		openpal::TimeDuration maxOpenRetry,
		const std::string& endpoint, 
		uint16_t port,
		opendnp3::IOpenDelayStrategy* pStrategy = opendnp3::ExponentialBackoffStrategy::Inst());

	/**
	* Add a serial channel
	*
	* @param logger Logger that will be used for all log messages	
	* @param minOpenRetry minimum connection retry interval on failure in milliseconds
	* @param maxOpenRetry minimum connection retry interval on failure in milliseconds
	* @param settings settings object that fully parameterizes the serial port
	* @param pStrategy Reconnection delay strategy, default to exponential
	*/
	opendnp3::IChannel* AddSerial(
		openpal::Logger,
		openpal::TimeDuration minOpenRetry,
		openpal::TimeDuration maxOpenRetry,
		asiopal::SerialSettings settings,
		opendnp3::IOpenDelayStrategy* pStrategy = opendnp3::ExponentialBackoffStrategy::Inst());

private:

	std::unique_ptr<asiopal::EventLog> mpLog;
	std::unique_ptr<asiopal::IOServiceThreadPool> mpThreadPool;
	std::unique_ptr<opendnp3::DNP3Manager> mpManager;
};

}

#endif
