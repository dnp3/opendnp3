//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#ifndef __APP_INTERFACES_H_
#define __APP_INTERFACES_H_


#include <stddef.h>

namespace apl
{
namespace dnp
{

class APDU;

/** Interface for the async app layer.

	For both transaction models defined below, the client is responsible
	for setting the header field of the  APDU, WITH THE EXCEPTION OF THE
	SEQUENCE #.
*/
class IAppLayer
{
public:

	/** Start a response transaction with optional confirmation.

		The incrementing of the sequence number is controlled
		by the FIR bit. If FIR is not set, the sequence is incremented.

		A callback to one of the following is guaranteed:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnSendSuccess() - The data was transmitted succesfully and valid confirmation
		was received within the timeout (if requested)
	*/
	virtual void SendResponse(APDU&) = 0;

	/** Start an unsolicited transaction with optional confirmation and retries. This
		sequence is almost identical to a response transaction.

		FIR and FIN must both be set for unsolcited responses since they cannot
		be multifragmented.

		A callback to one of the following is gauranteed:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnSendSucces() - The data was transmitted succesfully and valid confirmation
		was received within the timeout (if requested)
	*/
	virtual void SendUnsolicited(APDU&) = 0;

	/** Start a send transaction with optional confirmation and retries,
		that should result in a resposne.

		Callbacks are as follows:

		OnFailure() - The transaction failed, either because the send failed or
		the confirmation timed out.

		OnLowerLayerDown() - The transaction failed because the lower layer went down.

		OnPartialResponse(APDU) - A non-FIN response was received. The client should
		process the response and continue waiting for:

				- More partial responses
				- A failure
				- A final response

		OnFinalResponse(APDU) - A FIN response was received. The transaction is complete.
	*/
	virtual void SendRequest(APDU&) = 0;

	/**
		Cancel a running response transaction. The outstation must still wait for an OnFailure()
		from the application layer before proceeding with another response.

		This is necessary to implement the behavior decribed in in DNP3Spec-V2-Part1-ApplicationLayer:

		4.2.1 - ReadRules - Rule 1
	*/
	virtual void CancelResponse() = 0;
};


enum SequenceInfo {
	SI_OTHER,
	SI_PREV,
	SI_CORRECT
};

// Interface for callbacks from an application layer
class IAppUser
{
public:
	virtual void OnLowerLayerUp() = 0;					// The app layer is online
	virtual void OnLowerLayerDown() = 0;				// The app layer is offline

	virtual void OnUnsolSendSuccess() = 0;					// A Send operation has completed
	virtual void OnUnsolFailure() = 0;						// A transaction has failed for some reason

	virtual void OnSolSendSuccess() = 0;					// A Send operation has completed
	virtual void OnSolFailure() = 0;						// A transaction has failed for some reason

	virtual bool IsMaster() = 0;						// controls which types of messages are sent upwards

	// A non-final response has been received
	virtual void OnPartialResponse(const APDU&);
	// A final response has been received
	virtual void OnFinalResponse(const APDU&);

	// Process unsolicited data
	virtual void OnUnsolResponse(const APDU&);
	// Process request fragment
	virtual void OnRequest(const APDU&, SequenceInfo);
	// Unknown Object
	virtual void OnUnknownObject();
};

}
} //end ns

#endif
