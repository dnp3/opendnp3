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

#include "WriteTask.h"

using namespace openpal;

namespace opendnp3
{

WriteTask::WriteTask(const MasterParams& params, const std::string& name_, const std::function<void(HeaderWriter&)> format_, const openpal::Logger& logger) :
	NullResponseTask(logger),
	expiration(0),
	pParams(&params),
	name(name_),
	format(format_)
{

}

void WriteTask::BuildRequest(APDURequest& request, uint8_t seq)
{
	request.SetFunction(FunctionCode::WRITE);
	request.SetControl(AppControlField::Request(seq));
	auto writer = request.GetWriter();
	format(writer);
}

void WriteTask::OnLowerLayerClose(const openpal::MonotonicTimestamp&)
{
	expiration = MonotonicTimestamp::Max();
}

void WriteTask::OnResponseTimeout(const openpal::MonotonicTimestamp& now)
{
	expiration = MonotonicTimestamp::Max();
}

void WriteTask::OnSuccess(const openpal::MonotonicTimestamp&)
{
	expiration = MonotonicTimestamp::Max();
}

void WriteTask::OnBadControlOctet(const openpal::MonotonicTimestamp&)
{
	expiration = MonotonicTimestamp::Max();
}

void WriteTask::OnRejectedIIN(const openpal::MonotonicTimestamp&)
{
	expiration = MonotonicTimestamp::Max();
}

} //end ns

