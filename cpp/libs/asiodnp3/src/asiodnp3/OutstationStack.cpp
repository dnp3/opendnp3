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
#include "OutstationStack.h"

#include <asiopal/ASIOExecutor.h>

#include <secauth/outstation/OutstationAuthContext.h>

using namespace opendnp3;
using namespace secauth;

namespace asiodnp3
{

OutstationStack::OutstationStack(
	const char* id,
	openpal::LogRoot& root_,
    openpal::IExecutor& executor,	
	opendnp3::ICommandHandler& commandHandler,
	IOutstationApplication& application,	
    const OutstationStackConfig& config,
	IStackLifecycle& lifecycle) :
	
	root(root_, id),
	pLifecycle(&lifecycle),	
	stack(root, &executor, config.outstation.params.maxRxFragSize, &statistics, config.link),		
	ocontext(config.outstation, config.dbTemplate, root.GetLogger(), executor, stack.transport, commandHandler, application),
	outstation(ocontext)   
{
	stack.transport.SetAppLayer(&outstation);
}

opendnp3::DatabaseConfigView OutstationStack::GetConfigView()
{
	return outstation.GetConfigView();
}

void OutstationStack::SetRestartIIN()
{
	// this doesn't need to be synchronous, just post it
	auto lambda = [this]() { outstation.SetRestartIIN(); };
	pLifecycle->GetExecutor().strand.post(lambda);	
}

bool OutstationStack::Enable()
{
	return pLifecycle->EnableRoute(&stack.link);
}

bool OutstationStack::Disable()
{
	return pLifecycle->DisableRoute(&stack.link);
}

void OutstationStack::Shutdown()
{
	pLifecycle->Shutdown(&stack.link, this);	
}

StackStatistics OutstationStack::GetStackStatistics()
{	
	auto get = [this]() { return statistics; };
	return pLifecycle->GetExecutor().ReturnBlockFor<StackStatistics>(get);	
}

void OutstationStack::SetLinkRouter(opendnp3::ILinkRouter& router)
{
	stack.link.SetRouter(router);
}

opendnp3::ILinkSession& OutstationStack::GetLinkContext()
{
	return stack.link;
}

openpal::IExecutor& OutstationStack::GetExecutor()
{
	return pLifecycle->GetExecutor();
}

void OutstationStack::CheckForUpdates()
{
	outstation.CheckForUpdates();
}

}

