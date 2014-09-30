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
#ifndef OPENDNP3_ENABLEUNSOLICITEDTASK_H
#define OPENDNP3_ENABLEUNSOLICITEDTASK_H

#include "opendnp3/master/NullResponseTask.h"
#include "opendnp3/master/TaskPriority.h"

namespace openpal
{
	class IExecutor;
}

namespace opendnp3
{

/**
* Base class for tasks that only require a single response
*/

class EnableUnsolicitedTask : public NullResponseTask
{	

public:	

	EnableUnsolicitedTask(const MasterParams& params, openpal::Logger* pLogger_);

	virtual bool DeleteOnCompletion() override final { return false; }

	virtual char const* Name() const override final { return "Enable Unsolicited"; }

	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;		

	virtual int Priority() const override final { return priority::ENABLE_UNSOLICITED; }

	virtual openpal::MonotonicTimestamp ExpirationTime() const override final;

	virtual bool BlocksLowerPriority() const { return true; }

	virtual void OnLowerLayerClose(const openpal::MonotonicTimestamp& now) override final;

private:
	
	const MasterParams* pParams;

	openpal::MonotonicTimestamp expiration;

	virtual void OnSuccess(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnTimeoutOrBadControlOctet(const openpal::MonotonicTimestamp& now) override final;
};

} //end ns


#endif
