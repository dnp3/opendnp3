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
#ifndef OPENDNP3_USERPOLLTASK_H
#define OPENDNP3_USERPOLLTASK_H

#include "opendnp3/master/PollTaskBase.h"
#include "opendnp3/master/TaskPriority.h"
#include "opendnp3/master/ITaskCallback.h"
#include "opendnp3/gen/UserTaskResult.h"

#include <functional>

namespace opendnp3
{

class ISOEHandler;

/**
 * A generic interface for defining master request/response style tasks
 */

class UserPollTask : public PollTaskBase
{	

public:	

	UserPollTask(		
		const std::function<void(HeaderWriter&)>& builder,
		int id,
		bool recurring,
		const std::string& name,		
		const openpal::TimeDuration& period,	
		const openpal::TimeDuration& retryDelay,
		ISOEHandler* pSOEHandler,
		openpal::Logger* pLogger
		);	

	virtual int Priority() const override final { return priority::USER_POLL; }

	virtual TaskId Id() const override final { return TaskId::UserDefined(id); }

	virtual void BuildRequest(APDURequest& request, uint8_t seq) override final;

	virtual openpal::MonotonicTimestamp ExpirationTime() const override final { return expiration; }

	virtual bool BlocksLowerPriority() const override final { return false; }
	
	virtual bool IsRecurring() const override final { return recurring; }	

	virtual void OnLowerLayerClose(const openpal::MonotonicTimestamp&) override final;

	virtual void Demand() override final { expiration = 0; }

private:

	void Notify(UserTaskResult result);

	virtual void OnFailure(const openpal::MonotonicTimestamp& now) override final;

	virtual void OnSuccess(const openpal::MonotonicTimestamp& now) override final;
			
	std::function<void(HeaderWriter&)> builder;
	int id;
	bool recurring;
	openpal::TimeDuration period;
	openpal::TimeDuration retryDelay;
	openpal::MonotonicTimestamp expiration;
};


} //end ns


#endif
