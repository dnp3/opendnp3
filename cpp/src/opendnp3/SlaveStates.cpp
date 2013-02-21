
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
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#include "SlaveStates.h"

#include <opendnp3/Exception.h>
#include <opendnp3/Logger.h>

#include "Slave.h"
#include "AsyncTaskContinuous.h"
#include "LoggableMacros.h"

#include <sstream>

namespace opendnp3
{

void AS_Base::OnLowerLayerUp(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnLowerLayerDown(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnSolSendSuccess(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnSolFailure(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnUnsolSendSuccess(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnUnsolFailure(Slave*)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnRequest(Slave*, const APDU&, SequenceInfo)
{
	throw InvalidStateException(LOCATION, this->Name());
}

void AS_Base::OnUnknown(Slave* c)
{
	c->mDeferredUnknown = true;
}

// by default, the data update event is Deferred until we enter a state that can handle it
void AS_Base::OnDataUpdate(Slave* c)
{
	c->mDeferredUpdate = true;
}

// by default, the unsol timer expiration is deferd until it can be handled
void AS_Base::OnUnsolExpiration(Slave* c)
{
	c->mDeferredUnsol = true;
}

void AS_Base::SwitchOnFunction(Slave* c, AS_Base* apNext, const APDU& arRequest, SequenceInfo aSeqInfo)
{
	switch (arRequest.GetFunction()) {
	case (FC_READ): {
			ChangeState(c, apNext);
			c->mRspContext.Reset();
			IINField iin = c->mRspContext.Configure(arRequest);
			c->mRspContext.LoadResponse(c->mResponse);
			c->Send(c->mResponse, iin);
			break;
		}
	case (FC_WRITE):
		ChangeState(c, apNext);
		if(aSeqInfo != SI_PREV) c->HandleWrite(arRequest);
		c->ConfigureAndSendSimpleResponse();
		break;
	case (FC_PROPRIETARY_VTO_TRANSFER):
		ChangeState(c, apNext);
		if(aSeqInfo != SI_PREV) c->HandleVtoTransfer(arRequest);
		c->ConfigureAndSendSimpleResponse();
		break;
	case (FC_SELECT):
		ChangeState(c, apNext);
		c->HandleSelect(arRequest, aSeqInfo);
		c->Send(c->mResponse);
		break;
	case (FC_OPERATE):
		ChangeState(c, apNext);
		c->HandleOperate(arRequest, aSeqInfo);
		c->Send(c->mResponse);
		break;
	case (FC_DIRECT_OPERATE):
		ChangeState(c, apNext);
		c->HandleDirectOperate(arRequest, aSeqInfo);
		c->Send(c->mResponse);
		break;
	case (FC_DIRECT_OPERATE_NO_ACK):
		c->HandleDirectOperate(arRequest, aSeqInfo);
		break;
	case (FC_ENABLE_UNSOLICITED):
		ChangeState(c, apNext);
		c->HandleEnableUnsolicited(arRequest, true);
		c->Send(c->mResponse);
		break;
	case (FC_DISABLE_UNSOLICITED):
		ChangeState(c, apNext);
		c->HandleEnableUnsolicited(arRequest, false);
		c->Send(c->mResponse);
		break;
	case (FC_DELAY_MEASURE):
		ChangeState(c, apNext);
		c->ConfigureDelayMeasurement(arRequest);
		c->Send(c->mResponse);
		break;
	default: {
			std::ostringstream oss;
			oss << "Function not supported: " << arRequest.GetFunction();
			throw NotSupportedException(LOCATION, oss.str(), SERR_FUNC_NOT_SUPPORTED);
		}
	}
}

void AS_Base::DoRequest(Slave* c, AS_Base* apNext, const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	c->mRspIIN.Zero();

	try {
		this->SwitchOnFunction(c, apNext, arAPDU, aSeqInfo);
	}
	catch (ParameterException ex) {
		ChangeState(c, apNext);
		ERROR_LOGGER_BLOCK(c->mpLogger, LEV_ERROR, ex.Message(), ex.ErrorCode());
		c->mRspIIN.SetParameterError(true);
		c->ConfigureAndSendSimpleResponse();
	}
	catch (NotSupportedException ex) {
		ChangeState(c, apNext);
		ERROR_LOGGER_BLOCK(c->mpLogger, LEV_ERROR, ex.Message(), ex.ErrorCode());
		c->mRspIIN.SetFuncNotSupported(true);
		c->ConfigureAndSendSimpleResponse();
	}

	c->mLastRequest = arAPDU;
	c->mHaveLastRequest = true;
}

// Work functions

void AS_Base::ChangeState(Slave* c, AS_Base* apState)
{
	if (apState == AS_Closed::Inst() && c->mpTimeTimer) {
		c->mpTimeTimer->Cancel();
		c->mpTimeTimer = NULL;
	}
	LOGGER_BLOCK(c->mpLogger, LEV_DEBUG, "State changed from " << c->mpState->Name() << " to " << apState->Name());
	c->mpState = apState;
}

void AS_Base::DoUnsolSuccess(Slave* c)
{
	if (!c->mStartupNullUnsol) c->mStartupNullUnsol = true; //it was a null unsol packet
	c->mRspContext.ClearAndReset();

	// this will cause us to immediately re-evaluate if we need to send another unsol rsp
	// we use the Deferred mechanism to give the slave an opportunity to respond to any Deferred request instead
	c->mDeferredUnsol = true;
}

/* AS_Closed */

AS_Closed AS_Closed::mInstance;

void AS_Closed::OnLowerLayerUp(Slave* c)
{
	// this is implemented as a simple timer because it can run if the slave is connected/disconnected etc
	if (c->mConfig.mAllowTimeSync) c->ResetTimeIIN();

	ChangeState(c, AS_Idle::Inst());	
}

void AS_Closed::OnDataUpdate(Slave* c)
{
	c->FlushUpdates();
	if(!c->mConfig.mDisableUnsol) c->mDeferredUnsol = true;
}

/* AS_OpenBase */

void AS_OpenBase::OnLowerLayerDown(Slave* c)
{
	ChangeState(c, AS_Closed::Inst());
}

/* AS_Idle */

AS_Idle AS_Idle::mInstance;

void AS_Idle::OnRequest(Slave* c, const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	this->DoRequest(c, AS_WaitForRspSuccess::Inst(), arAPDU, aSeqInfo);
}

void AS_Idle::OnDataUpdate(Slave* c)
{
	c->FlushUpdates();

	// start the unsol timer or act immediately if there's no pack timer
	if (!c->mConfig.mDisableUnsol && c->mStartupNullUnsol && c->mRspContext.HasEvents(c->mConfig.mUnsolMask)) {
		if (c->mConfig.mUnsolPackDelay == 0) {			
			ChangeState(c, AS_WaitForUnsolSuccess::Inst());
			c->mRspContext.LoadUnsol(c->mUnsol, c->mIIN, c->mConfig.mUnsolMask);
			c->SendUnsolicited(c->mUnsol);
		}
		else if (c->mpUnsolTimer == NULL) {
			c->StartUnsolTimer(c->mConfig.mUnsolPackDelay);
		}
	}
}

void AS_Idle::OnUnsolExpiration(Slave* c)
{
	if (c->mStartupNullUnsol) {
		if (c->mRspContext.HasEvents(c->mConfig.mUnsolMask)) {
			ChangeState(c, AS_WaitForUnsolSuccess::Inst());
			c->mRspContext.LoadUnsol(c->mUnsol, c->mIIN, c->mConfig.mUnsolMask);
			c->SendUnsolicited(c->mUnsol);
		}
	}
	else {
		// do the startup null unsol task
		ChangeState(c, AS_WaitForUnsolSuccess::Inst());
		c->mRspContext.LoadUnsol(c->mUnsol, c->mIIN, ClassMask(false, false, false));
		c->SendUnsolicited(c->mUnsol);
	}
}

void AS_Idle::OnUnknown(Slave* c)
{
	c->HandleUnknown();
	ChangeState(c, AS_WaitForRspSuccess::Inst());
	c->Send(c->mResponse);
}

/* AS_WaitForRspSuccess */

AS_WaitForRspSuccess AS_WaitForRspSuccess::mInstance;

void AS_WaitForRspSuccess::OnSolFailure(Slave* c)
{
	ChangeState(c, AS_Idle::Inst());
	c->mRspContext.Reset();
}

void AS_WaitForRspSuccess::OnSolSendSuccess(Slave* c)
{
	c->mRspContext.ClearWritten();

	if (c->mRspContext.IsComplete()) {
		ChangeState(c, AS_Idle::Inst());
	}
	else {
		c->mRspContext.LoadResponse(c->mResponse);
		c->Send(c->mResponse);
	}
}

// When we get a request we should no longer wait for confirmation, but we should
// immediately handle the new request. We implement this behavior asynchronously, by
// canceling the response transaction, and waiting for an OnFailure callback.
// The callback may still succeed if
void AS_WaitForRspSuccess::OnRequest(Slave* c, const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	c->mpAppLayer->CancelResponse();
	c->mRequest = arAPDU;
	c->mSeqInfo = aSeqInfo;
	c->mDeferredRequest = true;
}

/* AS_WaitForUnsolSuccess */

AS_WaitForUnsolSuccess AS_WaitForUnsolSuccess::mInstance;

void AS_WaitForUnsolSuccess::OnUnsolFailure(Slave* c)
{
	// if any unsol transaction fails, we re-enable the timer with the unsol retry delay
	ChangeState(c, AS_Idle::Inst());
	c->mRspContext.Reset();
	c->StartUnsolTimer(c->mConfig.mUnsolRetryDelay);
}

void AS_WaitForUnsolSuccess::OnUnsolSendSuccess(Slave* c)
{
	ChangeState(c, AS_Idle::Inst());	// transition to the idle state
	this->DoUnsolSuccess(c);
}

void AS_WaitForUnsolSuccess::OnRequest(Slave* c, const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	if (arAPDU.GetFunction() == FC_READ) {
		//read requests should be defered until after the unsol
		c->mRequest = arAPDU;
		c->mSeqInfo = aSeqInfo;
		c->mDeferredRequest = true;
	}
	else {
		// all other requests should be handled immediately
		c->mDeferredRequest = false;
		this->DoRequest(c, AS_WaitForSolUnsolSuccess::Inst(), arAPDU, aSeqInfo);
	}
}

/* AS_WaitForSolUnsolSuccess */

AS_WaitForSolUnsolSuccess AS_WaitForSolUnsolSuccess::mInstance;

void AS_WaitForSolUnsolSuccess::OnRequest(Slave* c, const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	// Both channels are busy... buffer the request
	c->mRequest = arAPDU;
	c->mSeqInfo = aSeqInfo;
	c->mDeferredRequest = true;
}

void AS_WaitForSolUnsolSuccess::OnSolFailure(Slave* c)
{
	ChangeState(c, AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnSolSendSuccess(Slave* c)
{
	ChangeState(c, AS_WaitForUnsolSuccess::Inst());
}

void AS_WaitForSolUnsolSuccess::OnUnsolFailure(Slave* c)
{
	ChangeState(c, AS_WaitForRspSuccess::Inst());
	c->mRspContext.Reset();
	if (c->mConfig.mUnsolRetryDelay > 0)
		c->StartUnsolTimer(c->mConfig.mUnsolRetryDelay);
	else
		c->OnUnsolTimerExpiration();
}

void AS_WaitForSolUnsolSuccess::OnUnsolSendSuccess(Slave* c)
{
	ChangeState(c, AS_WaitForRspSuccess::Inst());
	this->DoUnsolSuccess(c);
}

} //ens ns

/* vim: set ts=4 sw=4: */
