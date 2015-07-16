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

#include <secauth/master/MasterAuthContext.h>

#include <asiopal/ASIOExecutor.h>

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
	IStackLifecycle& lifecycle,
	opendnp3::ITaskLock& taskLock) :
		root(root_, id),		
		pLifecycle(&lifecycle),
		stack(root, &executor, config.master.maxRxFragSize, &statistics, config.link),
		mcontext(new MContext(executor, root, stack.transport, SOEHandler, application,  config.master, taskLock)),
		master(*mcontext)
{
	stack.transport.SetAppLayer(&master);
}

MasterStackImpl::MasterStackImpl(
	const char* id,
	openpal::LogRoot& root_,
	asiopal::ASIOExecutor& executor,
	opendnp3::ISOEHandler& SOEHandler,
	opendnp3::IMasterApplication& application,
	const secauth::MasterAuthStackConfig& config,
	IStackLifecycle& lifecycle,
	opendnp3::ITaskLock& taskLock,
	secauth::IMasterUserDatabase& userDB,
	openpal::ICryptoProvider& crypto
) :
	root(root_, id),
	pLifecycle(&lifecycle),
	stack(root, &executor, config.master.maxRxFragSize, &statistics, config.link),
	mcontext(new secauth::MAuthContext(executor, root, stack.transport, SOEHandler, application, config.master, taskLock, config.auth, crypto, userDB)),
	master(*mcontext)
{

}

ICommandProcessor* MasterStackImpl::GetCommandProcessor()
{
	return &master.GetCommandProcessor();
}

bool MasterStackImpl::Enable()
{
	return pLifecycle->EnableRoute(&stack.link);
}

bool MasterStackImpl::Disable()
{
	return pLifecycle->DisableRoute(&stack.link);
}

void MasterStackImpl::Shutdown()
{
	return pLifecycle->Shutdown(&stack.link, this);	
}

StackStatistics MasterStackImpl::GetStackStatistics()
{	
	auto get = [this]() { return this->statistics; };
	return pLifecycle->GetExecutor().ReturnBlockFor<StackStatistics>(get);
}

void MasterStackImpl::SetLinkRouter(opendnp3::ILinkRouter& router)
{
	stack.link.SetRouter(router);
}

opendnp3::ILinkSession& MasterStackImpl::GetLinkContext()
{
	return stack.link;
}

opendnp3::MasterScan MasterStackImpl::AddScan(openpal::TimeDuration period, const std::vector<Header>& headers, const TaskConfig& config)
{
	auto func = ConvertToLambda(headers);
	return this->AddScan(period, func, config);
}

MasterScan MasterStackImpl::AddScan(TimeDuration period, const std::function<void(HeaderWriter&)>& builder, const TaskConfig& config)
{
	auto add = [this, period, builder, config]() { return master.AddScan(period, builder, config); };
	return pLifecycle->GetExecutor().ReturnBlockFor<MasterScan>(add);	
}

MasterScan MasterStackImpl::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period, const TaskConfig& config)
{
	auto add = [this, gvId, period, config]() { return master.AddAllObjectsScan(gvId, period, config); };
	return pLifecycle->GetExecutor().ReturnBlockFor<MasterScan>(add);
}

MasterScan MasterStackImpl::AddClassScan(const ClassField& field, openpal::TimeDuration period, const TaskConfig& config)
{	
	auto add = [this, field, period, config]() { return master.AddClassScan(field, period, config); };
	return pLifecycle->GetExecutor().ReturnBlockFor<MasterScan>(add);
}

MasterScan  MasterStackImpl::AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period, const TaskConfig& config)
{	
	auto add = [this, gvId, start, stop, period, config]() { return master.AddRangeScan(gvId, start, stop, period, config); };
	return pLifecycle->GetExecutor().ReturnBlockFor<MasterScan>(add);
}

void MasterStackImpl::Scan(const std::vector<Header>& headers, const TaskConfig& config)
{
	auto func = ConvertToLambda(headers);
	this->Scan(func, config);
}

void MasterStackImpl::Scan(const std::function<void(opendnp3::HeaderWriter&)>& builder, const TaskConfig& config)
{
	auto add = [this, builder, config]() { master.Scan(builder, config); };
	return pLifecycle->GetExecutor().BlockFor(add);
}

void MasterStackImpl::ScanAllObjects(opendnp3::GroupVariationID gvId, const TaskConfig& config)
{
	auto add = [this, gvId, config]() { master.ScanAllObjects(gvId, config); };
	return pLifecycle->GetExecutor().BlockFor(add);
}

void MasterStackImpl::ScanClasses(const opendnp3::ClassField& field, const TaskConfig& config)
{
	auto add = [this, field, config]() { master.ScanClasses(field, config); };
	return pLifecycle->GetExecutor().BlockFor(add);
}

void MasterStackImpl::ScanRange(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, const TaskConfig& config)
{
	auto add = [this, gvId, start, stop, config]() { master.ScanRange(gvId, start, stop, config); };
	return pLifecycle->GetExecutor().BlockFor(add);
}

void MasterStackImpl::Write(const TimeAndInterval& value, uint16_t index, const TaskConfig& config)
{
	auto add = [this, value, index, config]() { master.Write(value, index, config); };
	return pLifecycle->GetExecutor().BlockFor(add);
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
