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

#include "opendnp3/app/parsing/APDUHeaderParser.h"
#include "opendnp3/app/APDUBuilders.h"
#include "opendnp3/LogLevels.h"

#include "opendnp3/master/UserPollTask.h"
#include "opendnp3/master/WriteTask.h"
#include "opendnp3/master/MasterActions.h"

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

void Master::OnReceive(const openpal::ReadBufferView& apdu)
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

MasterScan Master::AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback, int userId)
{
	auto pTask = new UserPollTask(builder, true, period, context.mstate.params.taskRetryPeriod, *context.mstate.pApplication, *context.mstate.pSOEHandler, pCallback, userId, context.mstate.logger);
	context.ScheduleRecurringPollTask(pTask);	
	auto callback = [this]() { MasterActions::PostCheckForTask(this->context.mstate); };
	return MasterScan(*context.mstate.pExecutor, pTask, callback);
}

MasterScan Master::AddClassScan(const ClassField& field, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{	
	auto configure = [field](HeaderWriter& writer) 
	{ 
		build::WriteClassHeaders(writer, field); 
	};
	return this->AddScan(period, configure, pCallback, userId);
}

MasterScan Master::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId](HeaderWriter& writer)
	{ 
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);		
	};
	return this->AddScan(period, configure, pCallback, userId);
}

MasterScan Master::AddRangeScan(GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);		
	};
	return this->AddScan(period, configure, pCallback, userId);
}

void Master::Scan(const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback, int userId)
{
	auto pTask = new UserPollTask(builder, false, TimeDuration::Max(), context.mstate.params.taskRetryPeriod, *context.mstate.pApplication, *context.mstate.pSOEHandler, pCallback, userId, context.mstate.logger);
	context.ScheduleAdhocTask(pTask);	
}

void Master::ScanClasses(const ClassField& field, ITaskCallback* pCallback, int userId)
{
	auto configure = [field](HeaderWriter& writer)
	{
		build::WriteClassHeaders(writer, field);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::ScanAllObjects(GroupVariationID gvId, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId](HeaderWriter& writer)
	{
		writer.WriteHeader(gvId, QualifierCode::ALL_OBJECTS);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::ScanRange(GroupVariationID gvId, uint16_t start, uint16_t stop, ITaskCallback* pCallback, int userId)
{
	auto configure = [gvId, start, stop](HeaderWriter& writer)
	{
		writer.WriteRangeHeader<openpal::UInt16>(QualifierCode::UINT16_START_STOP, gvId, start, stop);
	};
	this->Scan(configure, pCallback, userId);
}

void Master::Write(const TimeAndInterval& value, uint16_t index, ITaskCallback* pCallback, int userId)
{
	auto format = [value, index](HeaderWriter& writer)
	{
		writer.WriteSingleIndexedValue<UInt16, TimeAndInterval>(QualifierCode::UINT16_CNT_UINT16_INDEX, Group50Var4::Inst(), value, index);
	};

	auto pTask = new WriteTask(*context.mstate.pApplication, format, context.mstate.logger, pCallback, userId);
	context.ScheduleAdhocTask(pTask);
}
	
}
