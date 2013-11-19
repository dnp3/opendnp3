
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

#ifndef __APP_CHANNEL_STATES_H_
#define __APP_CHANNEL_STATES_H_

#include <string>

#include <opendnp3/Singleton.h>
#include <opendnp3/APDUConstants.h>
#include <openpal/Visibility.h>

#include "AppLayerChannel.h"

namespace opendnp3
{

class APDU;


/** Defines interface that
	concrete states must implement/override - AppChannelState(ACS)_Base
*/
class DLL_LOCAL ACS_Base
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

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
	virtual std::string Name() const = 0;
#endif

	virtual bool AcceptsResponse() {
		return false;
	}

protected:

	void ThrowInvalidState(const std::string& arLocation);

	void ProcessResponse(AppLayerChannel*, APDU&, bool aExpectFIR);

};

class DLL_LOCAL ACS_Idle : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Idle)
	void Send(AppLayerChannel*, APDU&, size_t aNumRetry);

private:
	ACS_Base* NextState(AppLayerChannel* c, FunctionCodes, bool aConfirm);
};

//default implementations for failure and cancel events
class DLL_LOCAL ACS_SendBase : public ACS_Base
{
public:

	void OnSendFailure(AppLayerChannel*);
	void Cancel(AppLayerChannel* c);
};

class DLL_LOCAL ACS_Send : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_Send)

	void OnSendSuccess(AppLayerChannel* c);
};

class DLL_LOCAL ACS_SendConfirmed : public ACS_SendBase
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_SendConfirmed)

	void OnSendSuccess(AppLayerChannel*);
};

class DLL_LOCAL ACS_SendExpectResponse : public ACS_SendBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendExpectResponse);

	void OnSendSuccess(AppLayerChannel*);

};

class DLL_LOCAL ACS_SendCanceled : public ACS_Base
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_SendCanceled);

	void Cancel(AppLayerChannel*) {} //do nothing if we're canceled and we cancel again
	void OnSendSuccess(AppLayerChannel*);
	void OnSendFailure(AppLayerChannel*);
};

class DLL_LOCAL ACS_WaitForConfirm : public ACS_Base
{
	MACRO_NAME_SINGLETON_INSTANCE(ACS_WaitForConfirm)

	void Cancel(AppLayerChannel*);
	void OnConfirm(AppLayerChannel*, int aSequence);
	void OnTimeout(AppLayerChannel*);
};

class DLL_LOCAL ACS_WaitForResponseBase : public ACS_Base
{
public:
	void OnTimeout(AppLayerChannel*);
	bool AcceptsResponse() {
		return true;
	}
};



class DLL_LOCAL ACS_WaitForFirstResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFirstResponse);

	void OnResponse(AppLayerChannel*, APDU&);
};

class DLL_LOCAL ACS_WaitForFinalResponse : public ACS_WaitForResponseBase
{
	MACRO_STATE_SINGLETON_INSTANCE(ACS_WaitForFinalResponse);

	void OnResponse(AppLayerChannel*, APDU&);
};

}

#endif

