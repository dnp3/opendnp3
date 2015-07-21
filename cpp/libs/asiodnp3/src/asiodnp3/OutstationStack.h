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
#ifndef ASIODNP3_OUTSTATIONSTACK_H
#define ASIODNP3_OUTSTATIONSTACK_H

#include <opendnp3/outstation/OutstationStackConfig.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/Outstation.h>
#include <opendnp3/outstation/IOutstationApplication.h>
#include <opendnp3/link/ILinkRouter.h>
#include <opendnp3/transport/TransportStack.h>

#include <openpal/executor/IUTCTimeSource.h>
#include <openpal/crypto/ICryptoProvider.h>

#include "IStackLifecycle.h"
#include "IOutstation.h"
#include "ILinkBind.h"

#include <memory>

namespace asiodnp3
{

class ILinkSession;

/** @section desc A stack object for a master */
class OutstationStack: public IOutstation, public ILinkBind
{
public:

	/// standard DNP3 constructor
	OutstationStack(
		const char* id,
	    openpal::LogRoot&,
		openpal::IExecutor& executor,		
		opendnp3::ICommandHandler& commandHandler,
		opendnp3::IOutstationApplication& application,		
		const opendnp3::OutstationStackConfig& config,		
		IStackLifecycle& lifecycle);
	

	// ------- implement IOutstation -------

	virtual opendnp3::DatabaseConfigView GetConfigView() override final;	

	virtual void SetRestartIIN() override final;
	
	virtual bool Enable() override final;
	
	virtual bool Disable() override final;

	virtual void Shutdown() override final;

	virtual opendnp3::StackStatistics GetStackStatistics() override final;	

	// ------- implement ILinkBind ---------

	virtual void SetLinkRouter(opendnp3::ILinkRouter& router) override final;

	virtual opendnp3::ILinkSession& GetLinkContext() override final;

private:		

	virtual opendnp3::IDatabase& GetDatabase() override final { return outstation.GetDatabase(); }
	virtual openpal::IExecutor& GetExecutor() override final;
	virtual void CheckForUpdates() override final;

protected:

	openpal::LogRoot root;	
	opendnp3::StackStatistics statistics;	
	IStackLifecycle* pLifecycle;
	opendnp3::TransportStack stack;	
	opendnp3::OContext ocontext;
	opendnp3::Outstation outstation;
};

}

#endif

