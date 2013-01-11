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
#ifndef __APP_CHANNEL_STATES_H_
#define __APP_CHANNEL_STATES_H_

#include <APL/Singleton.h>
#include <string>

#include "APDUConstants.h"
#include "AppLayerChannel.h"

namespace apl
{
namespace dnp
{

class APDU;


/** Defines interface that
	concrete states must implement/override - AppChannelState(ACS)_Base
*/
class ACS_Base
{
public:

	virtual void Send(AppLayerChannel*, APDU&, size_t aNumRetry);
	virtual void Cancel(AppLayerChannel*); //cancel the outbound transaction

	// external events
	virtual void OnSendSuccess(AppLayerChannel*);
	virtual void OnSendFailure(AppLayerChannel*);
	virtual void OnConfirm(AppLayerChannel*, int aSequence);
	virtual void OnResponse(AppLayerChannel*, APDU&);
	virtual void OnTimeout(AppLayerChannel*);

	virtual std::string Name() const = 0;
	virtual bool AcceptsResponse() {
		return false;
	}

protected:

	void ThrowInvalidState(const std::string& arLocation);

	void ProcessResponse(AppLayerChannel*, APDU&, bool aExpectFIR);

};

class ACS_Idle : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Idle)
	void Send(AppLayerChannel*, APDU&, size_t aNumRetry);

private:
	ACS_Base* NextState(AppLayerChannel* c, FunctionCodes, bool aConfirm);
};

//default implementations for failure and cancel events
class ACS_SendBase : public ACS_Base
{
public:

	void OnSendFailure(AppLayerChannel*);
	void Cancel(AppLayerChannel* c);
};

class ACS_Send : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Send)

	void OnSendSuccess(AppLayerChannel* c);
};

class ACS_SendConfirmed : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_SendConfirmed)

	void OnSendSuccess(AppLayerChannel*);
};

class ACS_SendExpectResponse : public ACS_SendBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendExpectResponse);

	void OnSendSuccess(AppLayerChannel*);

};

class ACS_SendCanceled : public ACS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendCanceled);

	void Cancel(AppLayerChannel*) {} //do nothing if we're canceled and we cancel again
	void OnSendSuccess(AppLayerChannel*);
	void OnSendFailure(AppLayerChannel*);
};

class ACS_WaitForConfirm : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_WaitForConfirm)

	void Cancel(AppLayerChannel*);
	void OnConfirm(AppLayerChannel*, int aSequence);
	void OnTimeout(AppLayerChannel*);
};

class ACS_WaitForResponseBase : public ACS_Base
{
public:
	void OnTimeout(AppLayerChannel*);
	bool AcceptsResponse() {
		return true;
	}
};



class ACS_WaitForFirstResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFirstResponse);

	void OnResponse(AppLayerChannel*, APDU&);
};

class ACS_WaitForFinalResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFinalResponse);

	void OnResponse(AppLayerChannel*, APDU&);
};

}
}

#endif

