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
#include "Slave.h"

#include <opendnp3/AnalogOutput.h>
#include <opendnp3/ControlRelayOutputBlock.h>
#include <opendnp3/DNPConstants.h>

#include <openpal/LoggableMacros.h>
#include <openpal/IExecutor.h>

#include "SlaveStates.h"
#include "Database.h"
#include "APDUParser.h"
#include "IINHelpers.h"

#include "WriteHandler.h"
#include "ReadHandler.h"

#include <functional>

using namespace openpal;

namespace opendnp3
{

Slave::Slave(openpal::Logger aLogger, IAppLayer* apAppLayer, IExecutor* apExecutor, ITimeWriteHandler* apTimeWriteHandler, Database* apDatabase, ICommandHandler* apCmdHandler, const SlaveConfig& arCfg) :
	IAppUser(aLogger),
	StackBase(apExecutor),
	mpTimeWriteHandler(apTimeWriteHandler),
	mpAppLayer(apAppLayer),
	mpDatabase(apDatabase),
	mpCmdHandler(apCmdHandler),
	mpState(AS_Closed::Inst()),
	mConfig(arCfg),	
	mpUnsolTimer(nullptr),	
	mCachedRequest(arCfg.mMaxControls),	
	mRspContext(apDatabase, StaticResponseTypes(arCfg)),
	mSBOHandler(arCfg.mSelectTimeout, apCmdHandler, apExecutor),	
	mDeferredUpdateCount(0),	
	mDeferredUnsol(false),	
	mStartupNullUnsol(false),
	mState(StackState::COMMS_DOWN),
	mpTimeTimer(nullptr)
{
	/* Link the event buffer to the database */
	//mpDatabase->AddEventBuffer(mRspContext.GetBuffer());

	mIIN.Set(IINBit::DEVICE_RESTART);	// Always set on restart

	/*
	 * Incoming data will trigger a POST on the timer source to call
	 * Slave::OnDataUpdate().
	 */
	mChangeBuffer.AddObserver(mpExecutor, [this]() {
		this->OnDataUpdate();
	});

	/* Cause the slave to go through the null-unsol startup sequence */
	if (!mConfig.mDisableUnsol) {
		mDeferredUnsol = true;
	}
}

Slave::~Slave()
{
	if(mpUnsolTimer) mpUnsolTimer->Cancel();
	if(mpTimeTimer) mpTimeTimer->Cancel();
}

void Slave::SetNeedTimeIIN()
{
	mIIN.Set(IINBit::NEED_TIME);
}

void Slave::UpdateState(StackState aState)
{
	if(mState != aState) {
		LOG_BLOCK(LogLevel::Info, "StackState: " << StackStateToString(aState));
		mState = aState;
		this->NotifyListeners(aState);
	}
}

/* Implement IAppUser - external callbacks from the app layer */

void Slave::OnLowerLayerUp()
{
	mpState->OnLowerLayerUp(this);	
}

void Slave::OnLowerLayerDown()
{
	mpState->OnLowerLayerDown(this);	
	this->UpdateState(StackState::COMMS_DOWN);
}

void Slave::OnSolSendSuccess()
{
	mpState->OnSolSendSuccess(this);
	this->UpdateState(StackState::COMMS_UP);
}

void Slave::OnSolFailure()
{
	mpState->OnSolFailure(this);	
	LOG_BLOCK(LogLevel::Warning, "Response failure");
}

void Slave::OnUnsolSendSuccess()
{
	mpState->OnUnsolSendSuccess(this);	
	this->UpdateState(StackState::COMMS_UP);
}

void Slave::OnUnsolFailure()
{
	mpState->OnUnsolFailure(this);
	LOG_BLOCK(LogLevel::Warning, "Unsol response failure");	
}

void Slave::OnRequest(const APDURecord& record, SequenceInfo aSeqInfo)
{	
	mpState->OnRequest(this, record, aSeqInfo);	
}

/* Internally generated events */

void Slave::OnDataUpdate()
{
	// let the current state decide how to handle the change buffer
	mpState->OnDataUpdate(this);	
}

void Slave::OnUnsolTimerExpiration()
{
	// let the current state decide how to handle the timer expiration
	mpUnsolTimer = nullptr;
	mpState->OnUnsolExpiration(this);	
}


void Slave::ChangeState(SlaveStateBase* apState)
{	
	LOG_BLOCK(LogLevel::Debug, "State changed from " << mpState->Name() << " to " << apState->Name());
	mpState = apState;
	mpState->Enter(this);
}


/* Private functions */
size_t Slave::FlushUpdates()
{	
	mDeferredUpdateCount = 0;
	size_t num	= mChangeBuffer.FlushUpdates(mpDatabase);
	LOG_BLOCK(LogLevel::Debug, "Processed " << num << " updates");
	return num;		
}

void Slave::RespondToRequest(const APDURecord& record, SequenceInfo sequence)
{
	APDUResponse response(responseBuffer.GetWriteBuffer(mConfig.mMaxFragSize));
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField::DEFAULT);
	auto indications = ConfigureResponse(record, sequence, response);
	this->SendResponse(response, indications);	
}

IINField Slave::ConfigureResponse(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{	
	switch(request.function)
	{		
		case(FunctionCode::WRITE):			
			return HandleWrite(request, sequence);		
		case(FunctionCode::READ):			
			return HandleRead(request, sequence, response);		
		case(FunctionCode::DELAY_MEASURE):		
			return HandleDelayMeasure(request, sequence, response);		
		default:	
			ERROR_BLOCK(LogLevel::Warning, "Function not supported: " << FunctionCodeToString(request.function), SERR_FUNC_NOT_SUPPORTED);
			return IINField(IINBit::FUNC_NOT_SUPPORTED);			
	}
}

IINField Slave::HandleWrite(const APDURecord& request, SequenceInfo sequence)
{
	WriteHandler handler(mLogger);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler);
	if(result == APDUParser::Result::OK) return handler.Process(mIIN, 
		[this](const Group50Var1& absTime) { 
			mpExecutor->Post([this, absTime]() { mpTimeWriteHandler->WriteAbsoluteTime(absTime.time); });			
		}
	);
	else return IINFromParseResult(result);
}

IINField Slave::HandleRead(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{
	mRspContext.Reset();
	ReadHandler handler(mLogger, &mRspContext);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, APDUParser::Context(false)); // don't expect range/count context on a READ
	if(result == APDUParser::Result::OK)
	{
		auto errors = handler.Errors();
		if(errors.Any()) return errors;		
		else 
		{	
			// if the request contained static variations, we double buffer (copy) the entire static database.
			// this ensures that an multi-fragmented responses see a consistent snapshot
			if(!mRspContext.IsComplete()) mpDatabase->DoubleBuffer();
			auto result = this->mRspContext.Load(response); // todo get the overflow bits out of here & return them
			return IINField::Empty;
		}
	}
	else 
	{
		mRspContext.Reset();
		return IINFromParseResult(result);
	}
}

void Slave::ContinueResponse()
{
	APDUResponse response(responseBuffer.GetWriteBuffer(mConfig.mMaxFragSize));
	response.SetFunction(FunctionCode::RESPONSE);
	response.SetControl(AppControlField::DEFAULT);
	this->mRspContext.Load(response);
	this->SendResponse(response);
}

IINField Slave::HandleDelayMeasure(const APDURecord& request, SequenceInfo sequence, APDUResponse& response)
{		
	if(request.objects.IsEmpty()) 
	{	
		auto writer = response.GetWriter();
		Group52Var2 value = { 0 }; 	// respond with 0 time delay
		writer.WriteSingleValue<UInt8, Group52Var2>(QualifierCode::UINT8_CNT, value);		
		return IINField::Empty;
	}
	else
	{
		// there shouldn't be any trailing headers in delay measure request, no need to even parse
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}


void Slave::SendResponse(APDUResponse& response, const IINField& indications)
{
	response.SetIIN(mIIN | indications);
	mpAppLayer->SendResponse(response);
}	


/*
switch (record.function) 
{		
	case (FunctionCode::READ): 
	{
		ChangeState(slave, apNext);
		slave->mRspContext.Reset();
		IINField iin = slave->mRspContext.Configure(record);
		slave->mRspContext.LoadResponse(c->mResponse);
		slave->Send(c->mResponse, iin);
		break;
	}
	case (FunctionCode::WRITE):
	{
		ChangeState(slave, apNext);
		if(aSeqInfo != SI_PREV) slave->HandleWrite(record);
		c->ConfigureAndSendSimpleResponse();
		break;
	}
	case (FunctionCode::SELECT):
	{
		ChangeState(slave, apNext);
		c->HandleSelect(slave, aSeqInfo);
		c->Send(c->mResponse);
		break;
	}
	case (FunctionCode::OPERATE):
		ChangeState(slave, apNext);
		c->HandleOperate(arRequest, aSeqInfo);
		c->Send(c->mResponse);
		break;
	case (FunctionCode::DIRECT_OPERATE):
		ChangeState(slave, apNext);
		c->HandleDirectOperate(arRequest, aSeqInfo);
		c->Send(c->mResponse);
		break;
	case (FunctionCode::DIRECT_OPERATE_NO_ACK):
		c->HandleDirectOperate(arRequest, aSeqInfo);
		break;
	case (FunctionCode::ENABLE_UNSOLICITED):
		ChangeState(c, apNext);
		c->HandleEnableUnsolicited(arRequest, true);
		c->Send(c->mResponse);
		break;
	case (FunctionCode::DISABLE_UNSOLICITED):
		ChangeState(c, apNext);
		c->HandleEnableUnsolicited(arRequest, false);
		c->Send(c->mResponse);
		break;
	case (FunctionCode::DELAY_MEASURE):
		ChangeState(c, apNext);
		c->ConfigureDelayMeasurement(arRequest);
		c->Send(c->mResponse);
		break;
		
	default:
		ERROR_BLOCK(LogLevel::Warning, "Function not supported: " << FunctionCodeToString(record.function), SERR_FUNC_NOT_SUPPORTED);
		break;		
}
*/

/*
void Slave::SendUnsolicited(APDU& apdu, const IINField& indications)
{
	apdu.SetIIN(mIIN | indications);
	//mpAppLayer->SendUnsolicited(apdu); // TODO
}
*/

/*
void Slave::ConfigureDelayMeasurement(const APDU& arRequest)
{
	HeaderReadIterator hdr = arRequest.BeginRead();
	if (hdr.Count() > 0) {
		mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
	}

	Group52Var2Temp* pObj = Group52Var2Temp::Inst();

	mResponse.Set(FunctionCode::RESPONSE);

	IndexedWriteIterator i = mResponse.WriteIndexed(pObj, 1, QualifierCode::UINT8_CNT);
	i.SetIndex(0);
	pObj->mTime.Set(*i, 0);
}

void Slave::HandleWriteIIN(HeaderReadIterator& arHdr)
{
	for (ObjectReadIterator obj = arHdr.BeginRead(); !obj.IsEnd(); ++obj) {
		switch (obj->Index()) {
			case(static_cast<int>(IINBit::DEVICE_RESTART)): 
			{
				bool value = Group80Var1::Inst()->Read(*obj, obj->Start(), obj->Index());
				if (!value) {
					mIIN.Clear(IINBit::DEVICE_RESTART);
				}
				else {
					mRspIIN.Set(IINBit::PARAM_ERROR);
					ERROR_BLOCK(LogLevel::Warning, "", SERR_INVALID_IIN_WRITE);
				}
				break;
			}
		default:
			mRspIIN.Set(IINBit::PARAM_ERROR);
			ERROR_BLOCK(LogLevel::Warning, "", SERR_INVALID_IIN_WRITE);
			break;
		}
	}
}

void Slave::HandleWriteTimeDate(HeaderReadIterator& arHWI)
{
	if (mIIN.IsClear(IINBit::NEED_TIME)) {
		LOG_BLOCK(LogLevel::Warning, "Master is attempting to write time but slave is not requesting time sync");
		return;
	}

	ObjectReadIterator obj = arHWI.BeginRead();

	if (obj.Count() == 1) {

		auto utc = UTCTimestamp(Group50Var1::Inst()->mTime.Get(*obj));
		
		//make the callback with the stack unwound
		mpExecutor->Post([utc, this]() { mpTimeWriteHandler->WriteAbsoluteTime(utc); });

		mIIN.Clear(IINBit::NEED_TIME);

		ERROR_BLOCK(LogLevel::Event, "Time synchronized with master", TIME_SYNC_UPDATED);
	}
	else mRspIIN.Set(IINBit::PARAM_ERROR);	
}

void Slave::HandleWrite(const APDU& arRequest)
{
	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {
		case (MACRO_DNP_RADIX(80, 1)):
			this->HandleWriteIIN(hdr);
			break;
		case (MACRO_DNP_RADIX(50, 1)):
			this->HandleWriteTimeDate(hdr);
			break;
		default:
			mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
			ERROR_BLOCK(LogLevel::Warning, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleSelect(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	mResponse.Set(FunctionCode::RESPONSE);
	uint8_t seq = arRequest.GetControl().SEQ;

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();
		QualifierCode qual = i.Header().GetQualifier();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<ControlRelayOutputBlock>(Group12Var1::Inst(), i, [ = ](ControlRelayOutputBlock cmd, size_t idx) {
				return this->mSBOHandler.Select(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<AnalogOutputInt32>(Group41Var1::Inst(), i, [ = ](AnalogOutputInt32 cmd, size_t idx) {
				return this->mSBOHandler.Select(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<AnalogOutputInt16>(Group41Var2::Inst(), i, [ = ](AnalogOutputInt16 cmd, size_t idx) {
				return this->mSBOHandler.Select(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<AnalogOutputFloat32>(Group41Var3::Inst(), i, [ = ](AnalogOutputFloat32 cmd, size_t idx) {
				return this->mSBOHandler.Select(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<AnalogOutputDouble64>(Group41Var4::Inst(), i, [ = ](AnalogOutputDouble64 cmd, size_t idx) {
				return this->mSBOHandler.Select(cmd, idx, seq, qual);
			});
			break;

		default:
			mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
			ERROR_BLOCK(LogLevel::Warning, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleOperate(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	mResponse.Set(FunctionCode::RESPONSE);
	uint8_t seq = arRequest.GetControl().SEQ;

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();
		QualifierCode qual = i.Header().GetQualifier();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<ControlRelayOutputBlock>(Group12Var1::Inst(), i, [ = ](ControlRelayOutputBlock cmd, size_t idx) {
				return this->mSBOHandler.Operate(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<AnalogOutputInt32>(Group41Var1::Inst(), i, [ = ](AnalogOutputInt32 cmd, size_t idx) {
				return this->mSBOHandler.Operate(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<AnalogOutputInt16>(Group41Var2::Inst(), i, [ = ](AnalogOutputInt16 cmd, size_t idx) {
				return this->mSBOHandler.Operate(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<AnalogOutputFloat32>(Group41Var3::Inst(), i, [ = ](AnalogOutputFloat32 cmd, size_t idx) {
				return this->mSBOHandler.Operate(cmd, idx, seq, qual);
			});
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<AnalogOutputDouble64>(Group41Var4::Inst(), i, [ = ](AnalogOutputDouble64 cmd, size_t idx) {
				return this->mSBOHandler.Operate(cmd, idx, seq, qual);
			});
			break;

		default:
			mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
			ERROR_BLOCK(LogLevel::Warning, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleDirectOperate(const APDU& arRequest, SequenceInfo aSeqInfo)
{
	mResponse.Set(FunctionCode::RESPONSE);

	for (HeaderReadIterator hdr = arRequest.BeginRead(); !hdr.IsEnd(); ++hdr) {

		ObjectReadIterator i = hdr.BeginRead();

		switch (MACRO_DNP_RADIX(hdr->GetGroup(), hdr->GetVariation())) {

		case (MACRO_DNP_RADIX(12, 1)):
			this->RespondToCommands<ControlRelayOutputBlock>(Group12Var1::Inst(), i, [this](ControlRelayOutputBlock cmd, size_t idx) {
				return this->mpCmdHandler->DirectOperate(cmd, idx);
			});
			break;

		case (MACRO_DNP_RADIX(41, 1)):
			this->RespondToCommands<AnalogOutputInt32>(Group41Var1::Inst(), i, [this](AnalogOutputInt32 cmd, size_t idx) {
				return this->mpCmdHandler->DirectOperate(cmd, idx);
			});
			break;

		case (MACRO_DNP_RADIX(41, 2)):
			this->RespondToCommands<AnalogOutputInt16>(Group41Var2::Inst(), i, [this](AnalogOutputInt16 cmd, size_t idx) {
				return this->mpCmdHandler->DirectOperate(cmd, idx);
			});
			break;

		case (MACRO_DNP_RADIX(41, 3)):
			this->RespondToCommands<AnalogOutputFloat32>(Group41Var3::Inst(), i, [this](AnalogOutputFloat32 cmd, size_t idx) {
				return this->mpCmdHandler->DirectOperate(cmd, idx);
			});
			break;

		case (MACRO_DNP_RADIX(41, 4)):
			this->RespondToCommands<AnalogOutputDouble64>(Group41Var4::Inst(), i, [this](AnalogOutputDouble64 cmd, size_t idx) {
				return this->mpCmdHandler->DirectOperate(cmd, idx);
			});
			break;

		default:
			mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
			ERROR_BLOCK(LogLevel::Warning, "Object/Function mismatch", SERR_OBJ_FUNC_MISMATCH);
			break;
		}
	}
}

void Slave::HandleEnableUnsolicited(const APDU& arRequest, bool aIsEnable)
{
	mResponse.Set(FunctionCode::RESPONSE);

	if (mConfig.mDisableUnsol) {
		mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
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
				mRspIIN.Set(IINBit::FUNC_NOT_SUPPORTED);
				LOG_BLOCK(LogLevel::Warning, "Cannot enable/disable unsol for " << hdr->GetBaseObject()->Name());
				break;
			}
		}
	}
}
*/

void Slave::StartUnsolTimer(openpal::TimeDuration aTimeout)
{
	assert(mpUnsolTimer == nullptr);
	mpUnsolTimer = mpExecutor->Start(aTimeout, std::bind(&Slave::OnUnsolTimerExpiration, this));
}

void Slave::ResetTimeIIN()
{
	mpTimeTimer = nullptr;
	mIIN.Set(IINBit::NEED_TIME);
	mpTimeTimer = mpExecutor->Start(mConfig.mTimeSyncPeriod, std::bind(&Slave::ResetTimeIIN, this));
}

} //end ns


