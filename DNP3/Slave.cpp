
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
#include "Slave.h"


#include "SlaveStates.h"
#include "Database.h"
#include "DNPExceptions.h"
#include "ObjectReadIterator.h"

#include <APL/Logger.h>
#include <APL/AsyncTaskGroup.h>
#include <APL/AsyncTaskBase.h>
#include <APL/LoggableMacros.h>

#include <boost/bind.hpp>
#include <functional>

namespace apl
{
namespace dnp
{

Slave::Slave(Logger* apLogger, IAppLayer* apAppLayer, IExecutor* apExecutor, ITimeManager* apTime, Database* apDatabase, IDNPCommandMaster* apCmdMaster, const SlaveConfig& arCfg) :
	Loggable(apLogger),
	mpAppLayer(apAppLayer),
	mpExecutor(apExecutor),
	mpDatabase(apDatabase),
	mpCmdMaster(apCmdMaster),
	mpState(AS_Closed::Inst()),
	mConfig(arCfg),
	mRspTypes(arCfg),
	mpUnsolTimer(NULL),
	mResponse(arCfg.mMaxFragSize),
	mUnsol(arCfg.mMaxFragSize),
	mRspContext(apLogger, apDatabase, &mRspTypes, arCfg.mEventMaxConfig),
	mHaveLastRequest(false),
	mLastRequest(arCfg.mMaxFragSize),
	mpTime(apTime),
	mCommsStatus(apLogger, "comms_status"),
	mDeferredUpdate(false),
	mDeferredRequest(false),
	mDeferredUnsol(false),
	mDeferredUnknown(false),
	mStartupNullUnsol(false),
	mpObserver(arCfg.mpObserver),
	mState(SS_UNKNOWN),
	mpTimeTimer(NULL),
	mVtoReader(apLogger),
	mVtoWriter(apLogger->GetSubLogger("VtoWriter"), arCfg.mVtoWriterQueueSize)
{
	/* Link the event buffer to the database */
	mpDatabase->SetEventBuffer(mRspContext.GetBuffer());

	mIIN.SetDeviceRestart(true);	/* Always set on restart */

	/* Use the cmd master to send and rsp queue to wait for reply */
	mpCmdMaster->SetResponseObserver(&mRspQueue);

	/*
	 * Incoming data will trigger a POST on the timer source to call
	 * Slave::OnDataUpdate().
	 */
	mChangeBuffer.AddObserver(
	    mNotifierSource.Get(
	        std::bind(&Slave::OnDataUpdate, this),
	        mpExecutor
	    )
	);

	/*
	 * Incoming vto data will trigger a POST on the timer source to call
	 * Slave::OnVtoUpdate().
	 */
	mVtoWriter.AddObserver(
	    mNotifierSource.Get(
	        std::bind(&Slave::OnVtoUpdate, this),
	        mpExecutor
	    )
	);

	/* Cause the slave to go through the null-unsol startup sequence */
	if (!mConfig.mDisableUnsol) {
		mDeferredUnsol = true;
	}

	this->UpdateState(SS_COMMS_DOWN);
}

Slave::~Slave()
{
	if(mpUnsolTimer) mpUnsolTimer->Cancel();
	if(mpTimeTimer) mpTimeTimer->Cancel();

	mVtoWriter.RemoveObserver(mpVtoNotifier);
}

void Slave::UpdateState(StackStates aState)
{
	if(mState != aState) {
		mCommsStatus.Set(aState);
		LOG_BLOCK(LEV_INFO, "StackState: " << ConvertStackStateToString(aState));
		mState = aState;
		if(mpObserver != NULL) mpObserver->OnStateChange(aState);
	}
}

/* Implement IAppUser - external callbacks from the app layer */

void Slave::OnLowerLayerUp()
{
	mpState->OnLowerLayerUp(this);
	this->FlushDeferredEvents();	// all of the events check for Deferred events
}

void Slave::OnLowerLayerDown()
{
	mpState->OnLowerLayerDown(this);
	this->FlushDeferredEvents();
	this->UpdateState(SS_COMMS_DOWN);
}

void Slave::OnSolSendSuccess()
{
	mpState->OnSolSendSuccess(this);
	this->FlushDeferredEvents();
	this->UpdateState(SS_COMMS_UP);
}

void Slave::OnSolFailure()
{
	mpState->OnSolFailure(this);
	this->FlushDeferredEvents();
	LOG_BLOCK(LEV_WARNING, "Response failure");
}

void Slave::OnUnsolSendSuccess()
{
	mpState->OnUnsolSendSuccess(this);
	this->FlushDeferredEvents();
	this->UpdateState(SS_COMMS_UP);
}

void Slave::OnUnsolFailure()
{
	mpState->OnUnsolFailure(this);
	LOG_BLOCK(LEV_WARNING, "Unsol response failure");
	this->FlushDeferredEvents();
}

void Slave::OnRequest(const APDU& arAPDU, SequenceInfo aSeqInfo)
{
	mpState->OnRequest(this, arAPDU, aSeqInfo);
	this->FlushDeferredEvents();
}

void Slave::OnUnknownObject()
{
	mpState->OnUnknown(this);
	this->FlushDeferredEvents();
}

/* Internally generated events */

void Slave::OnVtoUpdate()
{
	/*
	 * Let the current state decide how to handle the VTO buffer.  We use the
	 * same handler as Slave::OnDataUpdate()
	 */
	this->OnDataUpdate();
}

void Slave::OnDataUpdate()
{
	// let the current state decide how to handle the change buffer
	//std::cout << "Got data update for: " << mpLogger->GetName() << " in state " << mpState->Name() << std::endl;
	mpState->OnDataUpdate(this);
	this->FlushDeferredEvents();
}

void Slave::OnUnsolTimerExpiration()
{
	// let the current state decide how to handle the timer expiration
	mpUnsolTimer = NULL;
	mpState->OnUnsolExpiration(this);
	this->FlushDeferredEvents();
}

/* Private functions */

void Slave::FlushDeferredEvents()
{
	/*
	 * If a data update events was previously Deferred this action might cause
	 * the state to change for an unsol response.
	 */
	if (mpState->AcceptsDeferredUpdates() && mDeferredUpdate) {
		mDeferredUpdate = false;
		mpState->OnDataUpdate(this);
	}

	/*
	 * If a request APDU was previously Deferred by a state, this action might
	 * cause a response and subsequent state change.
	 */
	if (mpState->AcceptsDeferredRequests() && mDeferredRequest) {
		mDeferredRequest = false;
		mpState->OnRequest(this, mRequest, mSeqInfo);
	}

	/*
	 * If an unsol timer expiration was Deferred by a state, this action might
	 * cause an unsolicted response to be generated.
	 */
	if (mpState->AcceptsDeferredUnsolExpiration() && mDeferredUnsol) {
		mDeferredUnsol = false;
		mpState->OnUnsolExpiration(this);
	}

	if (mpState->AcceptsDeferredUnknown() && mDeferredUnknown) {
		mDeferredUnknown = false;
		mpState->OnUnknown(this);
	}
}

size_t Slave::FlushVtoUpdates()
{
	/*
	 * Copy as much data as we can from the VtoWriter into the
	 * SlaveEventBuffer's VtoEvent buffer.
	 */
	IEventBuffer* pBuff = this->mRspContext.GetBuffer();
	size_t available = mVtoWriter.Size();
	size_t space = pBuff->NumVtoEventsAvailable();
	size_t flushed = this->mVtoWriter.Flush(pBuff, space);
	if(available > space) this->mDeferredUpdate = true;
	return flushed;
}

size_t Slave::FlushUpdates()
{
	size_t num = 0;
	try {
		Transaction t(&mChangeBuffer);
		num = mChangeBuffer.FlushUpdates(mpDatabase);
	}
	catch (Exception& ex) {
		LOG_BLOCK(LEV_ERROR, "Error in flush updates: " << ex.Message());
		Transaction tr(mChangeBuffer);
		mChangeBuffer.Clear();
		return 0;
	}

	num += this->FlushVtoUpdates();

	LOG_BLOCK(LEV_DEBUG, "Processed " << num << " updates");
	return num;
}


void Slave::ConfigureAndSendSimpleResponse()
{
	mResponse.Set(FC_RESPONSE);
	mRspIIN.BitwiseOR(mIIN);
	mResponse.SetIIN(mRspIIN);
	mpAppLayer->SendResponse(mResponse);
}

void Slave::Send(APDU& arAPDU, const IINField& arIIN)
{
	mRspIIN.BitwiseOR(mIIN);
	mRspIIN.BitwiseOR(arIIN);
	arAPDU.SetIIN(mRspIIN);
	mpAppLayer->SendResponse(arAPDU);
}

void Slave::Send(APDU& arAPDU)
{
	mRspIIN.BitwiseOR(mIIN);
	arAPDU.SetIIN(mRspIIN);
	mpAppLayer->SendResponse(arAPDU);
}

void Slave::SendUnsolicited(APDU& arAPDU)
{
	mRspIIN.BitwiseOR(mIIN);
	arAPDU.SetIIN(mRspIIN);
	mpAppLayer->SendUnsolicited(arAPDU);
}

void Slave::ConfigureDelayMeasurement(const APDU& arRequest)
{
	HeaderReadIterator hdr = arRequest.BeginRead();
	if (hdr.Count() > 0) {
		mRspIIN.SetFuncNotSupported(true);
	}

	Group52Var2* pObj = Group52Var2::Inst();

	mResponse.Set(FC_RESPONSE);

	IndexedWriteIterator i = mResponse.WriteIndexed(pObj, 1, QC_1B_CNT);
	i.SetIndex(0);
	pObj->mTime.Set(*i, 0);
}

void Slave::HandleWriteVto(HeaderReadIterator& arHdr)
{
	Transaction tr(mVtoReader);
	for (ObjectReadIterator obj = arHdr.BeginRead(); !obj.IsEnd(); ++obj) {
		size_t index = obj->Index();

		if(index > std::numeric_limits<uint8_t>::max()) {
			LOG_BLOCK(LEV_WARNING, "Ignoring VTO index that exceeds bit width of uint8_t: " << index);
		}
		else {
			/*
			 * Pass the data to the vto reader
			 */
			uint8_t channel = static_cast<uint8_t>(index);

			VtoData vto(arHdr->GetVariation());

			Group112Var0::Inst()->Read(*obj, arHdr->GetVariation(), vto.mpData);

			mVtoReader.Update(vto, channel);
		}
	}
}

void Slave::HandleWriteIIN(HeaderReadIterator& arHdr)
{
	for (ObjectReadIterator obj = arHdr.BeginRead(); !obj.IsEnd(); ++obj) {
		switch (obj->Index()) {
		case IINI_DEVICE_RESTART: {
				bool value = Group80Var1::Inst()->Read(*obj, obj->Start(), obj->Index());
				if (!value) {
					mIIN.SetDeviceRestart(false);
				}
				else {
					mRspIIN.SetParameterError(true);
					ERROR_BLOCK(LEV_WARNING, "", SERR_INVALID_IIN_WRITE);
				}
				break;
			}
		default:
			mRspIIN.SetParameterError(true);
			ERROR_BLOCK(LEV_WARNING, "", SERR_INVALID_IIN_WRITE);
			break;
		}
	}
}

void Slave::HandleWriteTimeDate(HeaderReadIterator& arHWI)
{
	if (!mIIN.GetNeedTime()) {
		LOG_BLOCK(LEV_WARNING, "Master is attempting to write time but slave is not requesting time sync");
		return;
	}

	ObjectReadIterator obj = arHWI.BeginRead();

	if (obj.Count() != 1) {
		mRspIIN.SetParameterError(true);
		return;
	}

	millis_t ms = Group50Var1::Inst()->mTime.Get(*obj);
	mpTime->SetTime(std::chrono::high_resolution_clock::time_point(std::chrono::milliseconds(ms)));

	mIIN.SetNeedTime(false);

	if(mpLogger->IsEnabled(LEV_EVENT)) {
		LogEntry le(LEV_EVENT, mpLogger->GetName(), LOCATION,
		            "Time synchronized with master", TIME_SYNC_UPDATED);
		le.AddValue("MILLISEC_SINCE_EPOCH", ms);
		mpLogger->Log(le);
	}
}

void Slave::HandleVtoTransfer(const APDU& arRequest)
{
	for(HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {
		switch(hdr->GetGroup()) {
			case 112:
				this->HandleWriteVto(hdr);
				break;
			default:
				mRspIIN.SetFuncNotSupported(true);
				ERROR_BLOCK(LEV_WARNING, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
				break;
		}
	}
}

void Slave::HandleWrite(const APDU& arRequest)
{	
	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {
		switch (hdr->GetGroup()) {
		case 112:
			this->HandleWriteVto(hdr);
			continue;
		}

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {
		case (MACRO_DNP_RADIX(80, 1)):
			this->HandleWriteIIN(hdr);
			break;
		case (MACRO_DNP_RADIX(50, 1)):
			this->HandleWriteTimeDate(hdr);
			break;
		default:
			mRspIIN.SetFuncNotSupported(true);
			ERROR_BLOCK(LEV_WARNING, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleSelect(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	mpCmdMaster->DeselectAll();

	mResponse.Set(FC_RESPONSE);

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<BinaryOutput>(Group12Var1::Inst(), i, boost::bind(&Slave::Select<BinaryOutput>, this, _1, _2, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<Setpoint>(Group41Var1::Inst(), i, boost::bind(&Slave::Select<Setpoint>, this, _1, _2, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<Setpoint>(Group41Var2::Inst(), i, boost::bind(&Slave::Select<Setpoint>, this, _1, _2, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<Setpoint>(Group41Var3::Inst(), i, boost::bind(&Slave::Select<Setpoint>, this, _1, _2, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<Setpoint>(Group41Var4::Inst(), i, boost::bind(&Slave::Select<Setpoint>, this, _1, _2, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		default:
			mRspIIN.SetFuncNotSupported(true);
			ERROR_BLOCK(LEV_WARNING, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleOperate(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	if (aSeqInfo == SI_PREV && mLastRequest == arRequest) {
		return;
	}

	mResponse.Set(FC_RESPONSE);

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<BinaryOutput>(Group12Var1::Inst(), i, boost::bind(&Slave::Operate<BinaryOutput>, this, _1, _2, false, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<Setpoint>(Group41Var1::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, false, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<Setpoint>(Group41Var2::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, false, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<Setpoint>(Group41Var3::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, false, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<Setpoint>(Group41Var4::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, false, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		default:
			mRspIIN.SetFuncNotSupported(true);
			ERROR_BLOCK(LEV_WARNING, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleDirectOperate(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	mResponse.Set(FC_RESPONSE);

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<BinaryOutput>(Group12Var1::Inst(), i, boost::bind(&Slave::Operate<BinaryOutput>, this, _1, _2, true, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<Setpoint>(Group41Var1::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, true, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<Setpoint>(Group41Var2::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, true, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<Setpoint>(Group41Var3::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, true, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<Setpoint>(Group41Var4::Inst(), i, boost::bind(&Slave::Operate<Setpoint>, this, _1, _2, true, hdr.info(), aSeqInfo, arRequest.GetControl().SEQ));
			break;

		default:
			mRspIIN.SetFuncNotSupported(true);
			ERROR_BLOCK(LEV_WARNING, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleEnableUnsolicited(const APDU& arRequest, bool aIsEnable)
{
	mResponse.Set(FC_RESPONSE);

	if (mConfig.mDisableUnsol) {
		mRspIIN.SetFuncNotSupported(true);
	}
	else {
		if (aIsEnable) {
			this->mDeferredUnsol = true;
		}

		for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

			switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {
			case (MACRO_DNP_RADIX(60, 2)):
				mConfig.mUnsolMask.class1 = aIsEnable;
				break;

			case (MACRO_DNP_RADIX(60, 3)):
				mConfig.mUnsolMask.class2 = aIsEnable;
				break;

			case (MACRO_DNP_RADIX(60, 4)):
				mConfig.mUnsolMask.class3 = aIsEnable;
				break;

			default:
				mRspIIN.SetFuncNotSupported(true);
				LOG_BLOCK(LEV_WARNING, "Cannot enable/disable unsol for " << hdr->GetBaseObject()->Name());
				break;
			}
		}
	}
}

void Slave::HandleUnknown()
{
	mResponse.Set(FC_RESPONSE);
	mRspIIN.SetObjectUnknown(true);
}

void Slave::StartUnsolTimer(millis_t aTimeout)
{
	assert(mpUnsolTimer == NULL);
	mpUnsolTimer = mpExecutor->Start(std::chrono::milliseconds(aTimeout), std::bind(&Slave::OnUnsolTimerExpiration, this));
}

void Slave::ResetTimeIIN()
{
	mpTimeTimer = NULL;
	mIIN.SetNeedTime(true);
	mpTimeTimer = mpExecutor->Start(std::chrono::milliseconds(mConfig.mTimeSyncPeriod), std::bind(&Slave::ResetTimeIIN, this));
}

}
} //end ns

/* vim: set ts=4 sw=4: */
