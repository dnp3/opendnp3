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
#ifndef __INTEGRITY_POLL_H_
#define __INTEGRITY_POLL_H_

#include "opendnp3/master/IMasterTask.h"

namespace opendnp3
{

class ISOEHandler;

/**
 * A generic interface for defining master request/response style tasks
 */
class IntegrityPoll : public IMasterTask
{	

public:	

	IntegrityPoll(ISOEHandler* pSOEHandler_, openpal::Logger* pLogger_, const MasterParams& params);
	
	virtual char const* Name() const override final;
	
	virtual TaskPriority Priority() const override final;
	
	virtual void BuildRequest(APDURequest& request) override final;
	
	virtual TaskStatus OnResponse(const APDUResponseRecord& response, IMasterScheduler& scheduler) override final;

	virtual void OnResponseTimeout(IMasterScheduler& scheduler) override final;
	

private:

	TaskStatus ProcessMeasurements(const APDUResponseRecord& response, IMasterScheduler& scheduler);

	ISOEHandler* pSOEHandler;
	openpal::Logger* pLogger;
	const MasterParams* pParams;

	uint16_t rxCount;
};

} //end ns


#endif
