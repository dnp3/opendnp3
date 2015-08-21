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

#include "EmptyResponseTask.h"

using namespace openpal;

namespace opendnp3
{

EmptyResponseTask::EmptyResponseTask(IMasterApplication& app, const std::string& name_, FunctionCode func_, const std::function<void(HeaderWriter&)> format_, openpal::Logger logger, ITaskCallback* pCallback, int userId) :
	IMasterTask(app, 0, logger, pCallback, userId),
	name(name_),
	func(func_),
	format(format_)
{

}

void EmptyResponseTask::BuildRequest(APDURequest& request, uint8_t seq)
{
	request.SetFunction(func);
	request.SetControl(AppControlField::Request(seq));
	auto writer = request.GetWriter();
	format(writer);
}

IMasterTask::ResponseResult EmptyResponseTask::_OnResponse(const opendnp3::APDUResponseHeader& header, const openpal::ReadBufferView& objects)
{
	return ValidateNullResponse(header, objects) ? ResponseResult::OK_FINAL : ResponseResult::ERROR_BAD_RESPONSE;
}

void EmptyResponseTask::_OnLowerLayerClose(openpal::MonotonicTimestamp now)
{
	
}

void EmptyResponseTask::_OnResponseTimeout(openpal::MonotonicTimestamp now)
{
	
}

void EmptyResponseTask::OnResponseOK(openpal::MonotonicTimestamp now)
{
	
}

void EmptyResponseTask::OnResponseError(openpal::MonotonicTimestamp now)
{
	
}

} //end ns

