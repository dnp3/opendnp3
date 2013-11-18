
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
	
	DNP3Manager();

	~DNP3Manager();
	
	IChannel* CreateChannel(openpal::Logger& arLogger, openpal::TimeDuration aOpenRetry, openpal::IPhysicalLayerAsync* apPhys);

	/**
	* Permanently shutdown the manager and all sub-objects that have been created. Stop
	* the thead pool.
	*/
	void Shutdown();

private:

	void OnChannelShutdownCallback(DNP3Channel* apChannel);	
	
	std::set<DNP3Channel*> mChannels;
};

}

#endif
