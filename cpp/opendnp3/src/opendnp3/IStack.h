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
#ifndef __I_STACK_H_
#define __I_STACK_H_

#include <functional>
#include <string>

#include <openpal/Logger.h>

#include "DestructorHook.h"
#include "gen/StackState.h"

namespace openpal
{
class IPhysicalLayerAsync;
}


namespace opendnp3
{

/**
* Base class for masters or outstations. Can be used to bind a vto endpoint or shutdown.
*/
class IStack : public DestructorHook
{
public:
	IStack(openpal::Logger& arLogger, std::function<void (bool)> aEnableDisableFunc);
	virtual ~IStack();

	/**
	* Add a listener for changes to the stack state. All callbacks come from the thread pool.
	* An immediate callback will be made with the current state.
	*
	* @param aListener Functor to call back with the state enumeration
	*/
	virtual void AddStateListener(std::function<void (StackState)> aListener) = 0;

	virtual openpal::IExecutor* GetExecutor() = 0;

	/**
	* Enable communications
	*/
	void Enable();

	/**
	* Enable communications
	*/
	void Disable();

	/**
	* Synchronously shutdown the endpoint
	* No more calls are allowed after this call.
	*/
	virtual void Shutdown() = 0;

private:

	openpal::Logger mLogger;
	std::function<void (bool)> mEnableDisableFunc;
};

}

#endif
