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

MasterStackImpl::MasterStackImpl(   const char* id,
									LogRoot& root_,
									asiopal::ASIOExecutor& executor,
									opendnp3::ISOEHandler& SOEHandler,                                    
									opendnp3::IMasterApplication& application,
                                    const MasterStackConfig& config,
                                    const StackActionHandler& handler_,									
									opendnp3::ITaskLock& taskLock) :
	root(root_, id),
	handler(handler_),
	stack(root, &executor, &statistics, config.link),
	master(executor, root, stack.transport, SOEHandler, application, config.master, taskLock)
{
	stack.transport.SetAppLayer(&master);
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

void MasterStackImpl::SetLinkRouter(opendnp3::ILinkRouter* pRouter)
{
	stack.link.SetRouter(pRouter);
}

void MasterStackImpl::SetShutdownAction(const openpal::Action0& action)
{
	shutdownAction = action;
}

opendnp3::ILinkContext* MasterStackImpl::GetLinkContext()
{
	return &stack.link;
}

MasterScan MasterStackImpl::AddAllObjectsScan(GroupVariationID gvId, openpal::TimeDuration period)
{
	auto add = [this, gvId, period]() { return master.AddAllObjectsScan(gvId, period); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

MasterScan MasterStackImpl::AddClassScan(const ClassField& field, openpal::TimeDuration period)
{	
	auto add = [this, field, period]() { return master.AddClassScan(field, period); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

MasterScan  MasterStackImpl::AddRangeScan(opendnp3::GroupVariationID gvId, uint16_t start, uint16_t stop, openpal::TimeDuration period)
{	
	auto add = [this, gvId, start, stop, period]() { return master.AddRangeScan(gvId, start, stop, period); };
	return asiopal::SynchronouslyGet<MasterScan>(handler.GetExecutor()->strand, add);
}

}
