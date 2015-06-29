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

#include "opendnp3/outstation/OutstationContext.h"

namespace opendnp3
{

/// Reusable actions that operate on oustation state
class OActions : private openpal::StaticOnly
{
	
	public:				
	
	static IINField GetResponseIIN(OContext& ocontext);	

	static void CheckForTaskStart(OContext& ocontext);	

	static void OnReceiveAPDU(OContext& ocontext, const openpal::ReadBufferView& apdu);	

	static void OnSendResult(OContext& ocontext, bool isSuccess);	

	/// ---- Processing functions --------

	static void ProcessHeaderAndObjects(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects);

	static void ProcessRequest(OContext& ocontext, const APDUHeader& header, const openpal::ReadBufferView& objects);	

	static void ProcessConfirm(OContext& ocontext, const APDUHeader& header);

	static void ProcessSolicitedConfirm(OContext& ocontext, const APDUHeader& header);

	static void ProcessUnsolicitedConfirm(OContext& ocontext, const APDUHeader& header);

	/// ---- Helper functions for begining solicited and unsolcited transmissions ----

	static void BeginResponseTx(OContext& ocontext, const openpal::ReadBufferView& response);

	static void BeginUnsolTx(OContext& ocontext, const openpal::ReadBufferView& response);

	static void BeginTx(OContext& ocontext, const openpal::ReadBufferView& response);
				

	static void CheckForDeferredRequest(OContext& ocontext);

	static bool ProcessDeferredRequest(OContext& ocontext, APDUHeader header, openpal::ReadBufferView objects);

	static bool StartSolicitedConfirmTimer(OContext& ocontext);

	static bool StartUnsolicitedConfirmTimer(OContext& ocontext);	

	static void CheckForUnsolicited(OContext& ocontext);

	static IINField GetDynamicIIN(OContext& ocontext);
};


}



#endif

