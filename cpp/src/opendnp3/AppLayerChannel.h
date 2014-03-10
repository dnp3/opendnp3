
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#ifndef __APP_LAYER_CHANNEL_H_
#define __APP_LAYER_CHANNEL_H_

#include <opendnp3/Types.h>
#include <opendnp3/Visibility.h>

#include "Loggable.h"

namespace opendnp3
{

class Logger;
class IExecutor;
class ITimer;
class AppLayer;
class ACS_Base;
class APDU;
struct AppControlField;

/**  The application layer contains two virtual channels, one for
	 solicited and unsolicited communication. Each channel has a sequence
	 number and some state associated with wether it is sending, waiting
	 for a response, etc
*/
class DLL_LOCAL AppLayerChannel : public Loggable
{
	friend class ACS_Base;
	friend class ACS_Idle;
	friend class ACS_Send;
	friend class ACS_SendBase;
	friend class ACS_SendConfirmed;
	friend class ACS_SendExpectResponse;
	friend class ACS_SendCanceled;
	friend class ACS_WaitForConfirm;
	friend class ACS_WaitForResponseBase;
	friend class ACS_WaitForFirstResponse;
	friend class ACS_WaitForFinalResponse;

public:
	AppLayerChannel(const std::string& arName, Logger*, AppLayer*, IExecutor*, millis_t aTimeout);
	virtual ~AppLayerChannel() {}

	// Resets the channel to the initial state
	void Reset();

	// send, wether a response is expected is implicit based on func code
	void Send(APDU&, size_t aNumRetry);
	void Cancel();
	void CancelRetries();

	// Events
	void OnSendSuccess();
	void OnSendFailure();
	void OnConfirm(int aSeq);

protected:

	// functions for the states to access
	int Sequence() {
		return mSequence;
	}
	int IncrSequence() {
		return mSequence = NextSeq(mSequence);
	}
	void QueueSend(APDU&);
	void ChangeState(ACS_Base*);
	void SetRetry(size_t aNumRetry) {
		mNumRetry = aNumRetry;
	}
	bool Retry(ACS_Base*);

	virtual void DoSendSuccess() = 0;
	virtual void DoFailure() = 0;

	void DoPartialResponse(APDU& arAPDU);
	void DoFinalResponse(APDU& arAPDU);

	void StartTimer();
	void CancelTimer();
	Logger* GetLogger() {
		return mpLogger;
	}

	AppLayer* mpAppLayer;
	int mSequence;	// Rotating sequence number for the channel
	ACS_Base* mpState;

	static int NextSeq(int s) {
		return (s + 1) % 16;
	}

private:

	void Timeout();

	APDU* mpSendAPDU;
	size_t mNumRetry;
	IExecutor* mpExecutor;
	ITimer* mpTimer;
	bool mConfirming;
	const millis_t M_TIMEOUT;
	const std::string M_NAME;
};

}

#endif
