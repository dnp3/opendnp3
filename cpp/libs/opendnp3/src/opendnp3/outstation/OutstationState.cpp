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

OutstationState::OutstationState(
		const OutstationParams& params_,
		openpal::IExecutor& executor,
		openpal::LogRoot& root,
		ILowerLayer& lower) :
	
	pExecutor(&executor),
	pLower(&lower),
	logger(root.GetLogger()),

	params(params_),	
	
	isOnline(false),
	isTransmitting(false),
	staticIIN(IINBit::DEVICE_RESTART),
	rxFragCount(0),
	pConfirmTimer(nullptr),

	operateExpectedSeq(0),
	operateExpectedFragCount(0),

	solSeqN(0),
	expectedSolConfirmSeq(0),
	pSolicitedState(&OutstationSolicitedStateIdle::Inst()),

	unsolSeqN(0),
	expectedUnsolConfirmSeq(0),
	completedNullUnsol(false),
	pUnsolicitedState(&OutstationUnsolicitedStateIdle::Inst())	
{	
	
}


}

