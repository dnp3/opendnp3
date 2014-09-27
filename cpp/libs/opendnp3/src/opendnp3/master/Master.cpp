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

#include "Master.h"

#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>

namespace opendnp3
{

Master::Master(
	openpal::IExecutor& executor,
	openpal::LogRoot& root,
	ILowerLayer& lower,
	ISOEHandler& SOEHandler,	
	opendnp3::IMasterApplication& application,
	const MasterParams& params,
	ITaskLock& taskLock
	) : 
	context(executor, root, lower, SOEHandler, application, params, taskLock),
	commandMarshaller(executor, context)
{}
	
void Master::OnLowerLayerUp()
{
	context.OnLayerUp();
}

void Master::OnLowerLayerDown()
{
	context.OnLayerDown();
}

void Master::OnReceive(const openpal::ReadOnlyBuffer& apdu)
{
	context.OnReceive(apdu);
}

void Master::OnSendResult(bool isSucccess)
{
	context.OnSendResult(isSucccess);	
}

ICommandProcessor& Master::GetCommandProcessor()
{
	return commandMarshaller;
}

MasterScan Master::AddScan(openpal::TimeDuration period, const std::function<void (APDURequest&)>& builder)
{
	//PollTask task(builder, period, context.pSOEHandler, &context.logger);
	//auto pTask = context.scheduler.AddPollTask(task);
	// todo crete the poll task
	return MasterScan(*context.pExecutor, nullptr);	
}

MasterScan Master::AddClassScan(const ClassField& field, openpal::TimeDuration period)
{	
	auto configure = [field](APDURequest& request) { build::WriteClassHeaders(request, field); };
	return this->AddScan(period, configure);
}

MasterScan Master::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period)
{
	auto configure = [gvId](APDURequest& request) { build::ReadAllObjects(request, gvId); };
	return this->AddScan(period, configure);
}

MasterScan Master::AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period)
{
	auto configure = [gvId, start, stop](APDURequest& request) {
		request.GetWriter().WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);		
	};
	return this->AddScan(period, configure);
}
	
}
