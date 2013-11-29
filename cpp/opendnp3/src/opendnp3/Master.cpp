
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

#include "Master.h"

#include <opendnp3/IDataObserver.h>

#include "MasterStates.h"
#include "ObjectReadIterator.h"
#include "ResponseLoader.h"
#include "ConstantCommandProcessor.h"
#include "AsyncTaskInterfaces.h"
#include "AsyncTaskGroup.h"
#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include <openpal/IExecutor.h>
#include "AsyncTaskContinuous.h"
#include "CopyableBuffer.h"

#include <functional>

using namespace openpal;

namespace opendnp3
{

Master::Master(Logger aLogger, MasterConfig aCfg, IAppLayer* apAppLayer, IMeasurementHandler* apPublisher, AsyncTaskGroup* apTaskGroup, openpal::IExecutor* apExecutor, IUTCTimeSource* apTimeSrc) :
	Loggable(aLogger),
	StackBase(apExecutor),
	mRequest(aCfg.FragSize),
	mpAppLayer(apAppLayer),
	mHandler(apPublisher, apExecutor),
	mpTaskGroup(apTaskGroup),
	mpTimeSrc(apTimeSrc),
	mpState(AMS_Closed::Inst()),
	mpTask(NULL),
	mpScheduledTask(NULL),
	mState(SS_COMMS_DOWN),
	mSchedule(apTaskGroup, this, aCfg),
	mClassPoll(aLogger, mHandler.Load),
	mClearRestart(aLogger),
	mConfigureUnsol(aLogger),
	mTimeSync(aLogger, apTimeSrc),
	mCommandTask(aLogger)
{
	/*
	 * Establish a link between the mCommandQueue and the
	 * mSchedule.mpCommandTask.  When new data is written to mCommandQueue,
	 * wake up mpCommandTask to process the data.
	 */
	mCommandQueue.AddObserver(mpExecutor, [this]() {
		this->mSchedule.mpCommandTask->Enable();
	});

}

void Master::UpdateState(StackState aState)
{
	if(mState != aState) {
		LOG_BLOCK(LEV_INFO, "StackState: " << ConvertStackStateToString(aState));
		mState = aState;
		this->NotifyListeners(aState);
	}
}

void Master::ProcessIIN(const IINField& arIIN)
{
	this->UpdateState(SS_COMMS_UP);

	bool check_state = false;

	//The clear IIN task only happens in response to detecting an IIN bit.
	if(arIIN.GetNeedTime()) {
		LOG_BLOCK(LEV_INFO, "Need time detected");
		mSchedule.mpTimeTask->SilentEnable();
		check_state = true;
	}

	if(mLastIIN.GetDeviceTrouble()) LOG_BLOCK(LEV_WARNING, "IIN Device trouble detected");

	if(mLastIIN.GetEventBufferOverflow()) LOG_BLOCK(LEV_WARNING, "Event buffer overflow detected");

	// If this is detected, we need to reset the startup tasks
	if(mLastIIN.GetDeviceRestart()) {
		LOG_BLOCK(LEV_WARNING, "Device restart detected");
		mSchedule.ResetStartupTasks();
		mSchedule.mpClearRestartTask->SilentEnable();
		check_state = true;
	}

	if(check_state) mpTaskGroup->CheckState();
}

void Master::ProcessCommand(ITask* apTask)
{
	if(mpState == AMS_Closed::Inst()) { //we're closed
		ConstantCommandProcessor ccp(mpExecutor, CommandResponse(CR_NO_COMMS));
		while(mCommandQueue.Dispatch(&ccp));
		apTask->Disable();
	}
	else {
		if(mCommandQueue.Dispatch(this)) {
			mpState->StartTask(this, apTask, &mCommandTask);
		}
		else apTask->Disable();
	}
}

void Master::SelectAndOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group12Var1::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::SELECT);
	this->mCommandTask.AddCommandCode(FunctionCode::OPERATE);
}

void Master::SelectAndOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var1::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::SELECT);
	this->mCommandTask.AddCommandCode(FunctionCode::OPERATE);
}

void Master::SelectAndOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var2::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::SELECT);
	this->mCommandTask.AddCommandCode(FunctionCode::OPERATE);
}

void Master::SelectAndOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var3::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::SELECT);
	this->mCommandTask.AddCommandCode(FunctionCode::OPERATE);
}

void Master::SelectAndOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var4::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::SELECT);
	this->mCommandTask.AddCommandCode(FunctionCode::OPERATE);
}

void Master::DirectOperate(const ControlRelayOutputBlock& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group12Var1::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::DIRECT_OPERATE);
}

void Master::DirectOperate(const AnalogOutputInt32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var1::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::DIRECT_OPERATE);
}

void Master::DirectOperate(const AnalogOutputInt16& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var2::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::DIRECT_OPERATE);
}

void Master::DirectOperate(const AnalogOutputFloat32& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var3::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::DIRECT_OPERATE);
}

void Master::DirectOperate(const AnalogOutputDouble64& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureCommandTask(arCommand, aIndex, Group41Var4::Inst(), aCallback);
	this->mCommandTask.AddCommandCode(FunctionCode::DIRECT_OPERATE);
}

void Master::StartTask(MasterTaskBase* apMasterTask, bool aInit)
{
	if(aInit) apMasterTask->Init();
	apMasterTask->ConfigureRequest(mRequest);
	mpAppLayer->SendRequest(mRequest);
}

/* Tasks */

void Master::SyncTime(ITask* apTask)
{
	if(mLastIIN.GetNeedTime()) {
		mpState->StartTask(this, apTask, &mTimeSync);
	}
	else apTask->Disable();
}

void Master::WriteIIN(ITask* apTask)
{
	if(mLastIIN.GetDeviceRestart()) {
		mpState->StartTask(this, apTask, &mClearRestart);
	}
	else apTask->Disable();
}

void Master::IntegrityPoll(ITask* apTask)
{
	mClassPoll.Set(PC_CLASS_0);
	mpState->StartTask(this, apTask, &mClassPoll);
}

void Master::EventPoll(ITask* apTask, int aClassMask)
{
	mClassPoll.Set(aClassMask);
	mpState->StartTask(this, apTask, &mClassPoll);
}

void Master::ChangeUnsol(ITask* apTask, bool aEnable, int aClassMask)
{
	mConfigureUnsol.Set(aEnable, aClassMask);
	mpState->StartTask(this, apTask, &mConfigureUnsol);
}

MasterScan Master::GetIntegrityScan()
{
	return MasterScan(mpExecutor, mSchedule.mpIntegrityPoll);
}

MasterScan Master::AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate)
{
	auto pTask = mSchedule.AddClassScan(aClassMask, aScanRate, aRetryRate);
	return MasterScan(mpExecutor, pTask);
}

/* Implement IAppUser */

void Master::OnLowerLayerUp()
{
	mpState->OnLowerLayerUp(this);
	mSchedule.EnableOnlineTasks();
}

void Master::OnLowerLayerDown()
{
	mpState->OnLowerLayerDown(this);
	mSchedule.DisableOnlineTasks();
	this->UpdateState(SS_COMMS_DOWN);
}

void Master::OnSolSendSuccess()
{
	mpState->OnSendSuccess(this);
}

void Master::OnSolFailure()
{
	this->UpdateState(SS_COMMS_DOWN);
	mpState->OnFailure(this);
}

void Master::OnUnsolSendSuccess()
{
	MACRO_THROW_EXCEPTION(InvalidStateException, "Master can't send unsol");
}

void Master::OnUnsolFailure()
{
	MACRO_THROW_EXCEPTION(InvalidStateException, "Master can't send unsol");
}

void Master::OnPartialResponse(const APDU& arAPDU)
{
	mLastIIN = arAPDU.GetIIN();
	this->ProcessIIN(mLastIIN);
	mpState->OnPartialResponse(this, arAPDU);
}

void Master::OnFinalResponse(const APDU& arAPDU)
{
	mLastIIN = arAPDU.GetIIN();
	this->ProcessIIN(arAPDU.GetIIN());
	mpState->OnFinalResponse(this, arAPDU);
}

void Master::OnUnsolResponse(const APDU& arAPDU)
{
	mLastIIN = arAPDU.GetIIN();
	this->ProcessIIN(mLastIIN);
	mpState->OnUnsolResponse(this, arAPDU);
}

/* Private functions */

void Master::ProcessDataResponse(const APDU& arResponse)
{
	try {
		ResponseLoader loader(this->mLogger, mHandler.Load);

		for(HeaderReadIterator hdr = arResponse.BeginRead(); !hdr.IsEnd(); ++hdr)
			loader.Process(hdr);
	}
	catch(const Exception& ex) {
		EXCEPTION_BLOCK(LEV_WARNING, ex)
	}
}

} //end ns

/* vim: set ts=4 sw=4: */
