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
#ifndef ASIODNP3_OUTSTATIONSTACKIMPL_H
#define ASIODNP3_OUTSTATIONSTACKIMPL_H

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/outstation/IOutstationAuthProvider.h>
#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/transport/TransportStack.h>

#include "IOutstation.h"
#include "Mutex.h"
#include "StackActionHandler.h"

#include <memory>

namespace asiodnp3
{

class ILinkSession;

/** @section desc A stack object for a master */
class OutstationStackImpl : public IOutstation
{
public:

	OutstationStackImpl(
		const char* id,
	    openpal::LogRoot&,
		openpal::IExecutor& executor,		
		opendnp3::ICommandHandler& commandHandler,
		opendnp3::IOutstationApplication& application,		
		const opendnp3::OutstationStackConfig& config,
	    const StackActionHandler& handler);	

	virtual opendnp3::DatabaseConfigView GetConfigView() override final;

	virtual opendnp3::IDatabase& GetDatabase() override final;

	virtual void SetRestartIIN() override final;
	
	virtual bool Enable() override final;
	
	virtual bool Disable() override final;

	virtual void Shutdown() override final;

	virtual opendnp3::StackStatistics GetStackStatistics() override final;	

	// ------- Non-interface public members ---------

	void SetLinkRouter(opendnp3::ILinkRouter& router);

	void SetShutdownAction(const openpal::Action0& action);

	opendnp3::ILinkSession* GetLinkContext();

private:
	Mutex mutex;
	openpal::LogRoot root;
	openpal::Action0 shutdownAction;
	opendnp3::StackStatistics statistics;	
	StackActionHandler handler;
	opendnp3::TransportStack stack;	
	std::unique_ptr<opendnp3::IOutstationAuthProvider> auth;
	opendnp3::Outstation outstation;
};

}

#endif

