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

#include "BeginUpdateKeyChangeTask.h"

#include <openpal/logging/LogMacros.h>

#include <opendnp3/objects/Group120.h>
#include <opendnp3/LogLevels.h>



using namespace openpal;
using namespace opendnp3;

namespace secauth
{

BeginUpdateKeyChangeTask::BeginUpdateKeyChangeTask(
		const std::string& username,
		IMasterApplicationSA& application,
		openpal::Logger logger,		
		const opendnp3::TaskConfig& config,
		openpal::ICryptoProvider& crypto,
		const BeginUpdateKeyChangeCallbackT& callback
	) : 
	IMasterTask(application, MonotonicTimestamp::Min(), logger, config),
	m_username(username),
	m_crypto(&crypto),
	m_callback(callback)
{

}
				
bool BeginUpdateKeyChangeTask::BuildRequest(opendnp3::APDURequest& request, uint8_t seq)
{
	request.SetControl(AppControlField::Request(seq));
	request.SetFunction(FunctionCode::AUTH_REQUEST);
	
	std::error_code ec;
	auto dest = m_challengeBuffer.GetWriteBuffer(4); // TODO - make this challenge size configurable
	this->m_challengeDataView = m_crypto->GetSecureRandom(dest, ec); 
	if (ec)
	{ 
		FORMAT_LOG_BLOCK(logger, flags::ERR, "Error creating master challenge data: %s", ec.message().c_str());
		return false;
	}

	Group120Var11 updateKeyChangeRequest(
		KeyChangeMethod::AES_256_SHA256_HMAC,
		openpal::ReadBufferView(reinterpret_cast<const uint8_t*>(m_username.c_str()), m_username.size()),
		m_challengeDataView
	);

	return request.GetWriter().WriteFreeFormat(updateKeyChangeRequest);
}

IMasterTask::ResponseResult BeginUpdateKeyChangeTask::ProcessResponse(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{	
	if (!(header.function == FunctionCode::AUTH_REQUEST && ValidateSingleResponse(header) && ValidateInternalIndications(header)))
	{
		return ResponseResult::ERROR_BAD_RESPONSE;
	}

		


	return ResponseResult::ERROR_BAD_RESPONSE;
}

IMasterTask::TaskState BeginUpdateKeyChangeTask::OnTaskComplete(opendnp3::TaskCompletion result, openpal::MonotonicTimestamp now)
{
	if (result != TaskCompletion::SUCCESS) // the success callback happens in the response routine if a valid response is received
	{		
		m_callback(BeginUpdateKeyChangeResult(result));
	}

	return TaskState::Infinite();
}

} //end ns
