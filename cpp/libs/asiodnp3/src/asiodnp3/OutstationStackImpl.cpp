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
#include "OutstationStackImpl.h"

#include <asiopal/ASIOExecutor.h>
#include <asiopal/StrandGetters.h>

using namespace opendnp3;

namespace asiodnp3
{

OutstationStackImpl::OutstationStackImpl(
	const char* id,
	openpal::LogRoot& root_,
    openpal::IExecutor& executor,	
	opendnp3::ICommandHandler& commandHandler,
	IOutstationApplication& application,
    const OutstationStackConfig& config,
    const StackActionHandler& handler_) :	
	
	root(root_, id),
	handler(handler_),
	stack(root, &executor, config.outstation.params.maxRxFragSize, &statistics, config.link),
	databaseBuffers(config.dbTemplate),
	eventBuffers(config.outstation.eventBufferConfig.TotalEvents()),
	mutex(),
	database(databaseBuffers.GetFacade(), &mutex),
	outstation(config.outstation, executor, root, stack.transport, commandHandler, application, database, eventBuffers.GetFacade())
{
	stack.transport.SetAppLayer(&outstation);
}

opendnp3::Database& OutstationStackImpl::GetDatabase()
{
	return database;
}

void OutstationStackImpl::SetRestartIIN()
{
	// this doesn't need to be synchronous, just post it
	auto lambda = [this]() { outstation.SetRestartIIN(); };
	handler.GetExecutor()->strand.post(lambda);	
}

bool OutstationStackImpl::Enable()
{
	return handler.EnableRoute(&stack.link);
}

bool OutstationStackImpl::Disable()
{
	return handler.DisableRoute(&stack.link);
}

void OutstationStackImpl::Shutdown()
{
	handler.Shutdown(&stack.link);
	shutdownAction.Apply();
}

StackStatistics OutstationStackImpl::GetStackStatistics()
{	
	auto getter = [this]() { return statistics; };
	return asiopal::SynchronouslyGet<StackStatistics>(handler.GetExecutor()->strand, getter);
}

void OutstationStackImpl::SetLinkRouter(opendnp3::ILinkRouter& router)
{
	stack.link.SetRouter(router);
}

void OutstationStackImpl::SetShutdownAction(const openpal::Action0& action)
{
	shutdownAction = action;
}

opendnp3::ILinkContext* OutstationStackImpl::GetLinkContext()
{
	return &stack.link;
}

}

