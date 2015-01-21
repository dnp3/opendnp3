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

#include "OutstationState.h"

using namespace openpal;

namespace opendnp3
{

	OState::OState(
		const OutstationConfig& config,
		const DatabaseTemplate& dbTemplate,
		openpal::IMutex* pMutex,
		INewEventDataHandler& handler,
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		ILowerLayer& lower,
		ICommandHandler& commandHandler,
		IOutstationApplication& application,
		IOutstationAuthProvider& authProvider) :
	
	pExecutor(&executor),
	pLower(&lower),
	logger(root.GetLogger()),
	pCommandHandler(&commandHandler),
	pApplication(&application),
	pAuthProvider(&authProvider),
	eventBuffer(config.eventBufferConfig),
	database(dbTemplate, eventBuffer, handler, config.params.indexMode, config.params.typesAllowedInClass0, pMutex),
	rspContext(database.GetStaticLoader(), eventBuffer),
	params(config.params),	
	isOnline(false),
	isTransmitting(false),
	pendingTaskCheckFlag(false),
	staticIIN(IINBit::DEVICE_RESTART),
	txBuffers(config.params.maxTxFragSize),
	deferred(config.params.maxRxFragSize),
	confirmTimer(executor)	
{	
	
}

void OState::Reset()
{
	isOnline = false;
	isTransmitting = false;
	pendingTaskCheckFlag = false;
	sol.pState = &OutstationSolicitedStateIdle::Inst();
	unsol.pState = &OutstationUnsolicitedStateIdle::Inst();
	history.Reset();
	deferred.Reset();
	eventBuffer.Unselect();
	rspContext.Reset();
	confirmTimer.Cancel();
}


}

