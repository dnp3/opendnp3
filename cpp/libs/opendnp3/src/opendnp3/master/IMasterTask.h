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
#ifndef OPENDNP3_IMASTERTASK_H
#define OPENDNP3_IMASTERTASK_H

#include <openpal/logging/Logger.h>

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/APDURequest.h"

#include "opendnp3/master/MasterParams.h"
#include "opendnp3/master/TaskResult.h"
#include "opendnp3/master/IMasterScheduler.h"

namespace opendnp3
{

class MasterTasks;

/**
 * A generic interface for defining master request/response style tasks
 */
class IMasterTask
{
	
public:

	/**	
	*
	* @return	the name of the task
	*/
	virtual char const* Name() const = 0;	

	/**
	 * Build a request APDU.
	 *	
	 */
	virtual void BuildRequest(APDURequest& request, const MasterParams& params, uint8_t seq) = 0;

	/**
	 * Handler for responses
	 *	 	 	
	 */
	virtual TaskStatus OnResponse(const APDUResponseHeader& response, const openpal::ReadOnlyBuffer& objects, const MasterParams& params, IMasterScheduler& scheduler) = 0;
	
	/**
	 * Called when a response times out
	 */
	virtual void OnResponseTimeout(const MasterParams& params, IMasterScheduler& scheduler) = 0;

	/**
	* Called when the layer closes while the task is executing
	*/
	virtual void OnLowerLayerClose() {}

	/**
	* Check if the task is enabled. Used for sequenced (startup) tasks.
	*
	* @return true if the task is enabled with the specified settings
	*/
	virtual bool Enabled(const MasterParams& params) { return false; }	

};

}

#endif
