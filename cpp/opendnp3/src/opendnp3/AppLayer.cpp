
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

#include "AppLayer.h"

#include <openpal/LoggableMacros.h>
#include <openpal/IExecutor.h>

#include "FunctionCodeHelpers.h"

using namespace std;

namespace opendnp3
{

AppLayer::AppLayer(Logger aLogger, openpal::IExecutor* apExecutor, AppConfig aAppCfg) :
	Loggable(aLogger),
	IUpperLayer(aLogger),
	mIncoming(aAppCfg.FragSize),
	mConfirm(2), // only need 2 bytes for a confirm message
	mSending(false),
	mConfirmSending(false),
	mpUser(nullptr),
	mSolicited(aLogger.GetSubLogger("sol"), this, apExecutor, aAppCfg.RspTimeout),
	mUnsolicited(aLogger.GetSubLogger("unsol"), this, apExecutor, aAppCfg.RspTimeout),
	mNumRetry(aAppCfg.NumRetry)
{
	mConfirm.SetFunction(FunctionCode::CONFIRM);
}

void AppLayer::SetUser(IAppUser* apUser)
{
	assert(mpUser == nullptr); assert(apUser != nullptr);
	mpUser = apUser;
}

////////////////////
// IAppLayer
////////////////////

void AppLayer::SendResponse(APDU& arAPDU)
{
	this->Validate(arAPDU.GetControl(), false, false, true, false);

	if(arAPDU.GetFunction() != FunctionCode::RESPONSE) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Non-response function code");
	}

	mSolicited.Send(arAPDU, this->GetRetries(FunctionCode::RESPONSE));
}

void AppLayer::SendUnsolicited(APDU& arAPDU)
{
	this->Validate(arAPDU.GetControl(), false, true, true, true);

	if(arAPDU.GetFunction() != FunctionCode::UNSOLICITED_RESPONSE ) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Non-unsolicited function code");
	}

	mUnsolicited.Send(arAPDU, this->GetRetries(FunctionCode::UNSOLICITED_RESPONSE));
}

void AppLayer::SendRequest(APDU& arAPDU)
{
	this->Validate(arAPDU.GetControl(), true, true, false, false);

	if(!IsRequest(arAPDU.GetFunction())) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Non-request function code");
	}

	mSolicited.Send(arAPDU, this->GetRetries(arAPDU.GetFunction()));
}

void AppLayer::CancelResponse()
{
	mSolicited.Cancel();
}

////////////////////
// External events
////////////////////

void AppLayer::_OnReceive(const uint8_t* apBuffer, size_t aSize)
{
	if(!this->IsLowerLayerUp()) {
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLaterDown");
	}

	try {
		mIncoming.Write(apBuffer, aSize);
		mIncoming.Interpret();

		LOG_BLOCK(LEV_INTERPRET, "<= AL " << mIncoming.ToString());

		FunctionCode func = mIncoming.GetFunction();
		AppControlField ctrl = mIncoming.GetControl();

		switch(func) {
		case(FunctionCode::CONFIRM):
			this->OnConfirm(ctrl, mIncoming);
			break;
		case(FunctionCode::RESPONSE):
			this->OnResponse(ctrl, mIncoming);
			break;
		case(FunctionCode::UNSOLICITED_RESPONSE):
			this->OnUnsolResponse(ctrl, mIncoming);
			break;
		default:	//otherwise, assume it's a request
			this->OnRequest(ctrl, mIncoming);
			break;
		}
	}
	catch(const ObjectException& oex) {
		EXCEPTION_BLOCK(LEV_WARNING, oex);
		this->OnUnknownObject(mIncoming.GetFunction(), mIncoming.GetControl());
	}
	catch(const Exception& ex) {
		EXCEPTION_BLOCK(LEV_WARNING, ex);
	}
}

void AppLayer::_OnLowerLayerUp()
{
	mpUser->OnLowerLayerUp();
}

void AppLayer::_OnLowerLayerDown()
{
	//reset both the channels
	mSolicited.Reset();
	mUnsolicited.Reset();

	//reset the transmitter state
	mSendQueue.erase(mSendQueue.begin(), mSendQueue.end());
	mSending = false;

	//notify the user
	mpUser->OnLowerLayerDown();
}

void AppLayer::OnSendResult(bool aSuccess)
{
	if(!mSending) {
		MACRO_THROW_EXCEPTION(InvalidStateException, "No Active Send");
	}

	assert(mSendQueue.size() > 0);
	mSending = false;

	FunctionCode func = mSendQueue.front()->GetFunction();
	mSendQueue.pop_front();

	if(func == FunctionCode::CONFIRM) {
		assert(mConfirmSending);
		mConfirmSending = false;
	}
	else {
		if(aSuccess) {
			if(func == FunctionCode::UNSOLICITED_RESPONSE) mUnsolicited.OnSendSuccess();
			else mSolicited.OnSendSuccess();
		}
		else {
			if(func == FunctionCode::UNSOLICITED_RESPONSE) mUnsolicited.OnSendFailure();
			else mSolicited.OnSendFailure();
		}
	}

	this->CheckForSend();
}

void AppLayer::_OnSendSuccess()
{
	this->OnSendResult(true);
}

void AppLayer::_OnSendFailure()
{
	this->OnSendResult(false);
}


////////////////////
// Internal Events
////////////////////

void AppLayer::OnResponse(const AppControlField& arCtrl, APDU& arAPDU)
{
	if(arCtrl.UNS) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "Bad unsol bit", ALERR_BAD_UNSOL_BIT);
	}

	// If we get a response that requests confirmation, we shouldn't confirm
	// if we're not going to handle the data. This is usually indicative of an
	// early timeout. It will show up in the logs as a response without context.
	if(arCtrl.CON && mSolicited.AcceptsResponse()) {
		this->QueueConfirm(false, arCtrl.SEQ);
	}

	mSolicited.OnResponse(arAPDU);
}

void AppLayer::OnUnsolResponse(const AppControlField& arCtrl, APDU& arAPDU)
{
	if(!arCtrl.UNS) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ALERR_BAD_UNSOL_BIT);
	}

	if(!mpUser->IsMaster())
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", SERR_FUNC_NOT_SUPPORTED);

	if(arCtrl.CON)
		this->QueueConfirm(true, arCtrl.SEQ);

	mUnsolicited.OnUnsol(arAPDU);
}

void AppLayer::OnConfirm(const AppControlField& arCtrl, APDU& arAPDU)
{
	arAPDU.Interpret(); //throws if there is additional data beyond length of 2

	// which channel?
	if(arCtrl.UNS) {
		if(mpUser->IsMaster()) {
			MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "", ALERR_UNEXPECTED_CONFIRM);
		}

		mUnsolicited.OnConfirm(arCtrl.SEQ);
	}
	else {
		mSolicited.OnConfirm(arCtrl.SEQ);
	}
}


void AppLayer::OnUnknownObject(FunctionCode aCode, const AppControlField& arCtrl)
{
	if(!mpUser->IsMaster()) {
		switch(aCode) {
		case(FunctionCode::CONFIRM):
		case(FunctionCode::RESPONSE):
		case(FunctionCode::UNSOLICITED_RESPONSE):
		case(FunctionCode::DIRECT_OPERATE_NO_ACK):
			break;
		default:
			mSolicited.OnUnknownObjectInRequest(arCtrl);
			mpUser->OnUnknownObject();
			break;
		}
	}
}

void AppLayer::OnRequest(const AppControlField& arCtrl, APDU& arAPDU)
{
	if(arCtrl.UNS) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "Received request with UNS bit", ALERR_BAD_UNSOL_BIT);
	}

	if(!(arCtrl.FIR && arCtrl.FIN)) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "Received non FIR/FIN request", ALERR_MULTI_FRAGEMENT_REQUEST);
	}

	if(mpUser->IsMaster()) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "Master received request apdu", MERR_FUNC_NOT_SUPPORTED);
	}

	mSolicited.OnRequest(arAPDU);
}

////////////////////
// Helperss
////////////////////

void AppLayer::QueueConfirm(bool aUnsol, int aSeq)
{
	if(mConfirmSending) {
		MACRO_THROW_EXCEPTION_WITH_CODE(Exception, "Unsol flood", aUnsol ? ALERR_UNSOL_FLOOD : ALERR_SOL_FLOOD);
	}

	mConfirmSending = true;
	mConfirm.SetControl(true, true, false, aUnsol, aSeq);

	this->QueueFrame(mConfirm);
}

void AppLayer::QueueFrame(const APDU& arAPDU)
{
	mSendQueue.push_back(&arAPDU);
	this->CheckForSend();
}

void AppLayer::CheckForSend()
{
	if(!mSending && mSendQueue.size() > 0) {
		mSending = true;
		const APDU* pAPDU = mSendQueue.front();
		LOG_BLOCK(LEV_INTERPRET, "=> AL " << pAPDU->ToString());
		mpLowerLayer->Send(pAPDU->GetBuffer(), pAPDU->Size());
	}
}

void AppLayer::Validate(const AppControlField& arCtrl, bool aMaster, bool aRequireFIRFIN, bool aAllowCON, bool aUNS)
{
	if(!this->IsLowerLayerUp()) {
		MACRO_THROW_EXCEPTION(InvalidStateException, "LowerLaterDown");
	}

	if(aMaster && !mpUser->IsMaster()) {
		MACRO_THROW_EXCEPTION(Exception, "Only masters can perform this operation");
	}

	if(!aMaster && mpUser->IsMaster()) {
		MACRO_THROW_EXCEPTION(Exception, "Only slaves can perform this operation");
	}

	if(aRequireFIRFIN && ! (arCtrl.FIR && arCtrl.FIN)) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Cannot be multi-fragmented");
	}

	if(!aAllowCON && arCtrl.CON) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Confirmation not allowed for this operation");
	}

	if(aUNS != arCtrl.UNS) {
		MACRO_THROW_EXCEPTION(ArgumentException, "Bad unsolicited bit");
	}
}

size_t AppLayer::GetRetries(FunctionCode aCode)
{
	switch(aCode) {
	case(FunctionCode::DIRECT_OPERATE):
	case(FunctionCode::DIRECT_OPERATE_NO_ACK):
	case(FunctionCode::RESPONSE):
	case(FunctionCode::WRITE): // b/c these can contain time objects which are sensitive to retries
		return 0;
	default:
		return mNumRetry; //use the configured
	}
}

} //end ns
