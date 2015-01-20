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
#ifndef OPENDNP3_OUTSTATIONSTATE_H
#define OPENDNP3_OUTSTATIONSTATE_H

#include "opendnp3/LayerInterfaces.h"

#include "opendnp3/outstation/OutstationParams.h"
#include "opendnp3/outstation/RequestHistory.h"
#include "opendnp3/outstation/DeferredRequest.h"
#include "opendnp3/outstation/OutstationChannelStates.h"
#include "opendnp3/outstation/ControlState.h"
#include "opendnp3/outstation/OutstationSeqNum.h"

#include <openpal/executor/TimerRef.h>
#include <openpal/logging/LogRoot.h>

namespace opendnp3
{

///
/// Represent all of the mutable state in an outstation
///
class OutstationState
{
	
	public:		

	OutstationState(const OutstationParams& params,										
					openpal::IExecutor& executor,
					openpal::LogRoot& root,
					ILowerLayer& lower);

	// reset important variables to their initial state
	void Reset();	

	// ------ External resources --------
	openpal::IExecutor* pExecutor;
	ILowerLayer* pLower;
	openpal::Logger logger;

	// ------ Static configuration -------
	OutstationParams params;	
		
	// ------ Shared dynamic state --------
	bool isOnline;
	bool isTransmitting;
	bool pendingTaskCheckFlag;
	IINField staticIIN;
	openpal::TimerRef confirmTimer;
	RequestHistory history;
	DeferredRequest deferred;

	// ------ Dynamic state related to controls ------
	ControlState control;

	// ------ Dynamic state related to solicited and unsolicited modes ------			
	OutstationSolState  sol;
	OutstationUnsolState unsol;
};


}

#endif
