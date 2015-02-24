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
#ifndef OPENDNP3_OUTSTATIONACTIONS_H
#define OPENDNP3_OUTSTATIONACTIONS_H

#include <openpal/container/Pair.h>
#include <openpal/util/Uncopyable.h>

#include "opendnp3/outstation/OutstationState.h"

namespace opendnp3
{

/// Reusable actions that operate on oustation state
class OActions : private openpal::PureStatic
{
	
	public:				

	static void OnNewEventData(OState& ostate);

	static IINField GetResponseIIN(OState& ostate);	

	static void CheckForTaskStart(OState& ostate);	

	static void OnReceiveAPDU(OState& ostate, const openpal::ReadBufferView& apdu);	

	static void OnSendResult(OState& ostate, bool isSuccess);	

	/// ---- Processing functions --------

	static void ProcessHeaderAndObjects(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	static void ProcessRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);	

	static void ProcessConfirm(OState& ostate, const APDUHeader& header);

	static void ProcessSolicitedConfirm(OState& ostate, const APDUHeader& header);

	static void ProcessUnsolicitedConfirm(OState& ostate, const APDUHeader& header);

	/// ---- Helper functions for begining solicited and unsolcited transmissions ----

	static void BeginResponseTx(OState& ostate, const openpal::ReadBufferView& response);

	static void BeginUnsolTx(OState& ostate, const openpal::ReadBufferView& response);

	static void BeginTx(OState& ostate, const openpal::ReadBufferView& response);
		

	/// ---- Helper functions that operate on the current solicited state, and may return a new solicited state ----

	static OutstationSolicitedStateBase* OnReceiveSolRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	static OutstationSolicitedStateBase* ContinueMultiFragResponse(OState& ostate, uint8_t seq);
	
	static OutstationSolicitedStateBase* RespondToNonReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	static OutstationSolicitedStateBase* RespondToReadRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);

	static OutstationSolicitedStateBase* ProcessNewRequest(OState& ostate, const APDUHeader& header, const openpal::ReadBufferView& objects);
	

	private:	

	static void CheckForDeferredRequest(OState& ostate);

	static bool ProcessDeferredRequest(OState& ostate, APDUHeader header, openpal::ReadBufferView objects);

	static bool StartSolicitedConfirmTimer(OState& ostate);

	static bool StartUnsolicitedConfirmTimer(OState& ostate);	

	static void CheckForUnsolicited(OState& ostate);

	static IINField GetDynamicIIN(OState& ostate);
};


}



#endif

