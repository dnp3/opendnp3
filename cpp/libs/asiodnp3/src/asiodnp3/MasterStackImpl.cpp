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
#include "MasterStackImpl.h"

#include <opendnp3/app/APDUBuilders.h>

#include <asiopal/ASIOExecutor.h>
#include <asiopal/StrandGetters.h>

using namespace openpal;
using namespace opendnp3;

namespace asiodnp3
{

MasterStackImpl::MasterStackImpl(   
	const char* id,
	LogRoot& root_,
	asiopal::ASIOExecutor& executor,
	opendnp3::ISOEHandler& SOEHandler,                                    
	opendnp3::IMasterApplication& application,
    const MasterStackConfig& config,
    const StackActionHandler& handler_,									
	opendnp3::ITaskLock& taskLock) :
		root(root_, id),
		handler(handler_),
		stack(root, &executor, config.master.maxRxFragSize, &statistics, config.link),
		master(executor, root, stack.transport, SOEHandler, application, config.master, taskLock)
{
	stack.transport.SetAppLayer(&master);
	stack.link.SetLinkStatusListener(this);
}

ICommandProcessor* MasterStackImpl::GetCommandProcessor()
{
	return &master.GetCommandProcessor();
}

bool MasterStackImpl::Enable()
{
	return handler.EnableRoute(&stack.link);
}

bool MasterStackImpl::Disable()
{
	return handler.DisableRoute(&stack.link);
}

void MasterStackImpl::Shutdown()
{
	handler.Shutdown(&stack.link);
	shutdownAction.Apply();
}

StackStatistics MasterStackImpl::GetStackStatistics()
{	
	auto get = [this]() { return this->statistics; };
	return asiopal::SynchronouslyGet<StackStatistics>(handler.GetExecutor()->strand, get);
}

void MasterStackImpl::SetLinkRouter(opendnp3::ILinkRouter& router)
{
	stack.link.SetRouter(router);
}

void MasterStackImpl::SetShutdownAction(const openpal::Action0& action)
{
	shutdownAction = action;
}

opendnp3::ILinkSession* MasterStackImpl::GetLinkContext()
{
	return &stack.link;
}

opendnp3::MasterScan MasterStackImpl::AddScan(openpal::TimeDuration period, const std::vector<Header>& headers, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto func = ConvertToLambda(headers);
	return this->AddScan(period, func, pCallback, userId);
}

MasterScan MasterStackImpl::AddScan(TimeDuration period, const std::function<void(HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, period, builder, pCallback, userId]() { return master.AddScan(period, builder, pCallback, userId); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

MasterScan MasterStackImpl::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, gvId, period, pCallback, userId]() { return master.AddAllObjectsScan(gvId, period, pCallback, userId); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

MasterScan MasterStackImpl::AddClassScan(const ClassField& field, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId)
{	
	auto add = [this, field, period, pCallback, userId]() { return master.AddClassScan(field, period, pCallback, userId); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

MasterScan  MasterStackImpl::AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, opendnp3::ITaskCallback* pCallback, int userId)
{	
	auto add = [this, gvId, start, stop, period, pCallback, userId]() { return master.AddRangeScan(gvId, start, stop, period, pCallback, userId); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::Scan(const std::vector<Header>& headers, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto func = ConvertToLambda(headers);
	this->Scan(func, pCallback, userId);
}

void MasterStackImpl::Scan(const std::function<void(opendnp3::HeaderWriter&)>& builder, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, builder, pCallback, userId]() { master.Scan(builder, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::ScanAllObjects(opendnp3::GroupVariationID gvId, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, gvId, pCallback, userId]() { master.ScanAllObjects(gvId, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::ScanClasses(const opendnp3::ClassField& field, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, field, pCallback, userId]() { master.ScanClasses(field, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, gvId, start, stop, pCallback, userId]() { master.ScanRange(gvId, start, stop, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::Write(const TimeAndInterval& value, uint16_t index, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, value, index, pCallback, userId]() { master.Write(value, index, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

void MasterStackImpl::EmptyResponseTask(const std::string& name, opendnp3::FunctionCode fc, opendnp3::ITaskCallback* pCallback, int userId)
{
	auto add = [this, name, fc, pCallback, userId]() { master.EmptyResponseTask(name, fc, [](HeaderWriter&){}, pCallback, userId); };
	return asiopal::SynchronouslyExecute(handler.GetExecutor()->strand, add);
}

std::function<void(opendnp3::HeaderWriter&)> MasterStackImpl::ConvertToLambda(const std::vector<Header>& headers)
{
	return[headers](opendnp3::HeaderWriter& writer){
		for (auto header : headers)
		{
			header.WriteTo(writer);
		}
	};
}

}
