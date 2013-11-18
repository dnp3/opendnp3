
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __DNP3_MANAGER_H_
#define __DNP3_MANAGER_H_

#include <string>
#include <set>
#include <stdint.h>
#include <memory>
#include <functional>

#include <openpal/Types.h>
#include <openpal/Logger.h>
#include <openpal/IPhysicalLayerAsync.h>
#include <openpal/TimeDuration.h>

#include "DestructorHook.h"

#ifndef OPENDNP3_NO_SERIAL
#include "SerialTypes.h"
#endif

/*! \mainpage opendnp3

\section Introduction

Companion documentation that describes theory of operation is located on the <a href="http://www.github.com/automatakllc/dnp3/wiki">project wiki</a>.

A simple application can be written as follows:

\code
#include <opendnp3/DNP3Manger.h>

using namespace opendnp3;

int main(int argc, char* argv[])
{
	DNP3Manager manager(1); // instantiate the root object specifying how many threads to use in the pool

	// start adding communication channels.... Check out the wiki.
}
\endcode

*/

// pre-declare EVERYTHING possible to minimize includes for CLR/Java wrappers
namespace opendnp3
{

class IOServiceThreadPool;
class EventLog;
class IChannel;
class DNP3Channel;

/**
The root class for all dnp3 applications. Used to retrieve communication channels on
which masters / outstations can be bound. The stack is event-based and driven by a thread pool.
All callbacks come from a thread in the pool.

\code
	// instantiate the root object specifying how many threads to use in the pool
	DNP3Manager manager(std::thread::hardware_concurrency());
\endcode

*/
class DNP3Manager : public DestructorHook
{
public:
	/** Constructor with optional callback paramters
	*   @param aConcurrency The number of threads allocated to the pool.
	*	@param aOnThreadStart Callback each thread will make before doing any work
	*   @param aOnThreadExit Callback each thread will make just before exiting
	*/
	DNP3Manager(
	        uint32_t aConcurrency,
	std::function<void()> aOnThreadStart = []() {},
	std::function<void()> aOnThreadExit = []() {}
	);

	~DNP3Manager();

	/**
	* Add a callback to receive log messages
	* @param apLog Pointer to a callback object
	*/
	void AddLogSubscriber(openpal::ILogBase* apLog);

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

	/**
	* Add a tcp client channel
	*
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on failure in milliseconds
	* @param arHost IP address of remote outstation (i.e. 127.0.0.1 or www.google.com)
	* @param aPort Port of remote outstation is listening on
	*/
	IChannel* AddTCPClient(const std::string& arLoggerId, openpal::FilterLevel aLevel, openpal::TimeDuration aOpenRetry, const std::string& arHost, uint16_t aPort);

	/**
	* Add a tcp server channel
	*
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on bind failure in milliseconds
	* @param arEndpoint Network adapter to listen on, i.e. 127.0.0.1 or 0.0.0.0
	* @param aPort Port to listen on
	*/
	IChannel* AddTCPServer(const std::string& arLoggerId, openpal::FilterLevel aLevel, openpal::TimeDuration aOpenRetry, const std::string& arEndpoint, uint16_t aPort);

#ifndef OPENDNP3_NO_SERIAL
	/**
	* Add a serial channel
	* @param arLoggerId name that will be used in all log messages
	* @param aLevel lowest log level of all messages
	* @param aOpenRetry connection retry interval on open failure in milliseconds
	* @param aSettings settings object that fully parameterizes the serial port
	*/
	IChannel* AddSerial(const std::string& arLoggerId, openpal::FilterLevel aLevel, openpal::TimeDuration aOpenRetry, SerialSettings aSettings);
#endif

private:

	void OnChannelShutdownCallback(DNP3Channel* apChannel);

	IChannel* CreateChannel(openpal::Logger& arLogger, openpal::TimeDuration aOpenRetry, openpal::IPhysicalLayerAsync* apPhys);

	std::auto_ptr<EventLog> mpLog;
	std::auto_ptr<IOServiceThreadPool> mpThreadPool;
	std::set<DNP3Channel*> mChannels;
};

}

#endif
