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
#include "opendnp3/master/MeasurementHandler.h"

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
	mcontext(executor, root, lower, SOEHandler, application, *this, params, taskLock),
	commandProcessor(mcontext)
{}
	
void Master::OnLowerLayerUp()
{
	mcontext.GoOnline();
}

void Master::OnLowerLayerDown()
{
	mcontext.GoOffline();
}

void Master::OnReceive(const openpal::ReadBufferView& apdu)
{
	if (!mcontext.isOnline)
	{
		SIMPLE_LOG_BLOCK(mcontext.logger, flags::ERR, "Ignorning rx data while offline");
		return;
	}

	APDUResponseHeader header;
	if (!APDUHeaderParser::ParseResponse(apdu, header, &mcontext.logger))
	{
		return;
	}


	FORMAT_LOG_BLOCK(mcontext.logger, flags::APP_HEADER_RX,
		"FIR: %i FIN: %i CON: %i UNS: %i SEQ: %i FUNC: %s IIN: [0x%02x, 0x%02x]",
		header.control.FIR,
		header.control.FIN,
		header.control.CON,
		header.control.UNS,
		header.control.SEQ,
		FunctionCodeToString(header.function),
		header.IIN.LSB,
		header.IIN.MSB);


	mcontext.OnReceive(apdu, header, apdu.Skip(APDU_RESPONSE_HEADER_SIZE));
}

void Master::OnPendingTask()
{	
	mcontext.PostCheckForTask();
}

void Master::OnSendResult(bool isSucccess)
{
	if (mcontext.isOnline && mcontext.isSending)
	{
		mcontext.isSending = false;

		mcontext.CheckConfirmTransmit();
		mcontext.CheckForTask();
	}
}

ICommandProcessor& Master::GetCommandProcessor()
{
	return commandProcessor;
}

void Master::SetAuthProvider(IMasterAuthProvider& provider)
{
	this->mcontext.auth.SetProvider(mcontext, provider);
}

MasterScan Master::AddScan(openpal::TimeDuration period, const std::function<void(HeaderWriter&)>& builder, ITaskCallback* pCallback, int userId)
{
	auto pTask = new UserPollTask(builder, true, period, mcontext.params.taskRetryPeriod, *mcontext.pApplication, *mcontext.pSOEHandler, pCallback, userId, mcontext.logger);
	this->ScheduleRecurringPollTask(pTask);	
	auto callback = [this]() { this->mcontext.PostCheckForTask(); };
	return MasterScan(*mcontext.pExecutor, pTask, callback);
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
	auto pTask = new UserPollTask(builder, false, TimeDuration::Max(), mcontext.params.taskRetryPeriod, *mcontext.pApplication, *mcontext.pSOEHandler, pCallback, userId, mcontext.logger);
	this->ScheduleAdhocTask(pTask);	
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

	auto pTask = new WriteTask(*mcontext.pApplication, format, mcontext.logger, pCallback, userId);
	this->ScheduleAdhocTask(pTask);
}

/// ------ private helpers ----------

void Master::ScheduleRecurringPollTask(IMasterTask* pTask)
{
	mcontext.tasks.BindTask(pTask);

	if (mcontext.isOnline)
	{
		mcontext.scheduler.Schedule(ManagedPtr<IMasterTask>::WrapperOnly(pTask));
		mcontext.PostCheckForTask();
	}
}

void Master::ScheduleAdhocTask(IMasterTask* pTask)
{
	auto task = ManagedPtr<IMasterTask>::Deleted(pTask);
	if (mcontext.isOnline)
	{
		mcontext.scheduler.Schedule(std::move(task));
		mcontext.PostCheckForTask();
	}
	else
	{
		// can't run this task since we're offline so fail it immediately
		pTask->OnLowerLayerClose(mcontext.pExecutor->GetTime());
	}
}
	
}
