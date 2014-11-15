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

#include "opendnp3/master/UserPollTask.h"
#include "opendnp3/master/WriteTask.h"

#include <openpal/logging/LogMacros.h>

using namespace openpal;

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

MasterScan Master::AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, int id)
{
	auto pTask = new UserPollTask(builder, id, true, "", period, context.params.taskRetryPeriod, context.pSOEHandler, context.logger);
	context.ScheduleRecurringPollTask(pTask);	
	auto callback = [this]() { this->context.PostCheckForTask(); };
	return MasterScan(*context.pExecutor, pTask, callback);
}

MasterScan Master::AddClassScan(const ClassField& field, openpal::TimeDuration period, int id)
{	
	auto configure = [field](HeaderWriter& writer) 
	{ 
		build::WriteClassHeaders(writer, field); 
	};
	return this->AddScan(period, configure, id);
}

MasterScan Master::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, int id)
{
	auto configure = [gvId](HeaderWriter& writer)
	{ 
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);		
	};
	return this->AddScan(period, configure, id);
}

MasterScan Master::AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, int id)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);		
	};
	return this->AddScan(period, configure, id);
}

void Master::Scan(const std::function<void(HeaderWriter&)>& builder, int id)
{
	auto pTask = new UserPollTask(builder, id, false, "", TimeDuration::Max(), context.params.taskRetryPeriod, context.pSOEHandler, context.logger);
	context.ScheduleAdhocTask(pTask);	
}

void Master::ScanClasses(const ClassField& field, int id)
{
	auto configure = [field](HeaderWriter& writer)
	{
		build::WriteClassHeaders(writer, field);
	};
	this->Scan(configure, id);
}

void Master::ScanAllObjects(GroupVariationID gvId, int id)
{
	auto configure = [gvId](HeaderWriter& writer)
	{
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);
	};
	this->Scan(configure, id);
}

void Master::ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, int id)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);
	};
	this->Scan(configure, id);
}

void Master::Write(const TimeAndInterval& value, uint16_t index, int id)
{
	auto format = [value, index](HeaderWriter& writer)
	{
		writer.WriteSingleIndexedValue<UInt16, TimeAndInterval>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group50Var4::Inst(), value, index);
	};

	auto pTask = new WriteTask(context.params, "", TaskId::UserDefined(id), format, context.logger);

	context.ScheduleAdhocTask(pTask);
}
	
}
