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

#include "FinishUpdateKeyChangeTask.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{


FinishUpdateKeyChangeTask::FinishUpdateKeyChangeTask(
	const FinishUpdateKeyChangeArgs& args,
	IMasterApplicationSA& application,
	openpal::Logger logger,
	const opendnp3::TaskConfig& config,
	openpal::ICryptoProvider& crypto
) :
	IMasterTask(application, MonotonicTimestamp::Min(), logger, config),
	m_arguments(args),
	m_crypto(&crypto)
{}
			

bool FinishUpdateKeyChangeTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	return false;
}

IMasterTask::ResponseResult FinishUpdateKeyChangeTask::ProcessResponse(const opendnp3::APDUResponseHeader& response, const openpal::ReadBufferView& objects)
{
	return IMasterTask::ResponseResult::ERROR_BAD_RESPONSE;
}

IMasterTask::TaskState FinishUpdateKeyChangeTask::OnTaskComplete(opendnp3::TaskCompletion result, openpal::MonotonicTimestamp now)
{
	return IMasterTask::TaskState::Infinite();
}
	

} //end ns



