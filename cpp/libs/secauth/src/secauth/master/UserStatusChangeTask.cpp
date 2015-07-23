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

#include "UserStatusChangeTask.h"

#include <opendnp3/objects/Group120.h>

using namespace opendnp3;

namespace secauth
{

	UserStatusChangeTask::UserStatusChangeTask(
			const UserStatusChange& userStatusChange,
			opendnp3::IMasterApplication& application,
			openpal::Logger logger,
			const opendnp3::TaskConfig& config
		) :
		IMasterTask(application, openpal::MonotonicTimestamp::Min(), logger, config),
		statusChange(userStatusChange)
	{}
					
	void UserStatusChangeTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
	{		
		request.SetControl(AppControlField::Request(seq));
		request.SetFunction(FunctionCode::AUTH_REQUEST);
		request.GetWriter().WriteFreeFormat(statusChange.Convert());
	}
	

	opendnp3::IMasterTask::ResponseResult UserStatusChangeTask::_OnResponse(const opendnp3::APDUResponseHeader& response, const openpal::ReadBufferView& objects)
	{
		/// TODO, actually parse the response
		return ResponseResult::ERROR_BAD_RESPONSE;
	}	

} //end ns



