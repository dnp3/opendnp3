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
#include "Master.h"

#include <opendnp3/IDataObserver.h>

#include "MasterStates.h"
#include "MeasurementHandler.h"
#include "ConstantCommandProcessor.h"
#include "AsyncTaskInterfaces.h"
#include "AsyncTaskGroup.h"
#include "AsyncTaskBase.h"
#include "AsyncTaskPeriodic.h"
#include "AsyncTaskNonPeriodic.h"
#include <openpal/IExecutor.h>
#include "AsyncTaskContinuous.h"
#include "CopyableBuffer.h"
#include "APDUParser.h"

#include <openpal/Exception.h>
#include <openpal/LoggableMacros.h>

#include <functional>

using namespace openpal;

namespace opendnp3
{

Master::Master(Logger aLogger, MasterConfig aCfg, IAppLayer* apAppLayer, ISOEHandler* apSOEHandler, AsyncTaskGroup* apTaskGroup, openpal::IExecutor* apExecutor, IUTCTimeSource* apTimeSrc) :
	IAppUser(aLogger),
	StackBase(apExecutor),	
	mpAppLayer(apAppLayer),	
	mpSOEHandler(apSOEHandler),
	mpTaskGroup(apTaskGroup),
	mpTimeSrc(apTimeSrc),
	mpState(AMS_Closed::Inst()),
	mpTask(nullptr),
	mpScheduledTask(nullptr),
	mState(StackState::COMMS_DOWN),
	mSchedule(apTaskGroup, this, aCfg),
	mClassPoll(aLogger, apSOEHandler),
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
		LOG_BLOCK(LogLevel::Info, "StackState: " << StackStateToString(aState));
		mState = aState;
		this->NotifyListeners(aState);
	}
}

void Master::ProcessIIN(const IINField& arIIN)
{
	this->UpdateState(StackState::COMMS_UP);

	bool check_state = false;

	//The clear IIN task only happens in response to detecting an IIN bit.
	if(mLastIIN.IsSet(IINBit::NEED_TIME)) {
		LOG_BLOCK(LogLevel::Info, "Need time detected");
		mSchedule.mpTimeTask->SilentEnable();
		check_state = true;
	}

	if(mLastIIN.IsSet(IINBit::DEVICE_TROUBLE)) LOG_BLOCK(LogLevel::Warning, "IIN Device trouble detected");

	if(mLastIIN.IsSet(IINBit::EVENT_BUFFER_OVERFLOW)) LOG_BLOCK(LogLevel::Warning, "Event buffer overflow detected");

	// If this is detected, we need to reset the startup tasks
	if(mLastIIN.IsSet(IINBit::DEVICE_RESTART)) {
		LOG_BLOCK(LogLevel::Warning, "Device restart detected");
		mSchedule.ResetStartupTasks();
		mSchedule.mpClearRestartTask->SilentEnable();
		check_state = true;
	}

	if(check_state) mpTaskGroup->CheckState();
}

void Master::ProcessCommand(ITask* apTask)
{
	if(mpState == AMS_Closed::Inst()) { //we're closed
		ConstantCommandProcessor ccp(mpExecutor, CommandResponse(CommandResult::NO_COMMS));
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

void Master::SelectAndOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureSBO(arCommand, aIndex, aCallback);
}

void Master::SelectAndOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureSBO(arCommand, aIndex, aCallback);
}

void Master::SelectAndOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureSBO(arCommand, aIndex, aCallback);
}

void Master::SelectAndOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureSBO(arCommand, aIndex, aCallback);
}

void Master::SelectAndOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureSBO(arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const ControlRelayOutputBlock& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureDO(arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const AnalogOutputInt32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureDO(arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const AnalogOutputInt16& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureDO(arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const AnalogOutputFloat32& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureDO(arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const AnalogOutputDouble64& arCommand, uint16_t aIndex, std::function<void(CommandResponse)> aCallback)
{
	this->mCommandTask.ConfigureDO(arCommand, aIndex, aCallback);
}

/* TODO
void Master::StartTask(MasterTaskBase* apMasterTask, bool aInit)
{
	if(aInit) apMasterTask->Init();
	apMasterTask->ConfigureRequest(mRequest);
	//mpAppLayer->SendRequest(mRequest); // TODO restore transmission
}
*/

/* Tasks */

void Master::SyncTime(ITask* apTask)
{
	if(mLastIIN.IsSet(IINBit::NEED_TIME)) {
		mpState->StartTask(this, apTask, &mTimeSync);
	}
	else apTask->Disable();
}

void Master::WriteIIN(ITask* apTask)
{
	if(mLastIIN.IsSet(IINBit::DEVICE_RESTART)) {
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
	this->UpdateState(StackState::COMMS_DOWN);
}

void Master::OnSolSendSuccess()
{
	mpState->OnSendSuccess(this);
}

void Master::OnSolFailure()
{
	this->UpdateState(StackState::COMMS_DOWN);
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

void Master::OnPartialResponse(const APDUResponseRecord& aRecord)
{
	mLastIIN =  aRecord.IIN;
	this->ProcessIIN(mLastIIN);
	mpState->OnPartialResponse(this, aRecord);
}

void Master::OnFinalResponse(const APDUResponseRecord& aRecord)
{
	mLastIIN = aRecord.IIN;
	this->ProcessIIN(mLastIIN);
	mpState->OnFinalResponse(this, aRecord);
}

void Master::OnUnsolResponse(const APDUResponseRecord& aRecord)
{
	mLastIIN = aRecord.IIN;
	this->ProcessIIN(mLastIIN);
	mpState->OnUnsolResponse(this, aRecord);
}

/* Private functions */

void Master::ProcessDataResponse(const APDUResponseRecord& record)
{
	MeasurementHandler handler(mLogger, this->mpSOEHandler);
	auto res = APDUParser::ParseTwoPass(record.objects, &handler);
	if(res != APDUParser::Result::OK) {	
		LOG_BLOCK(LogLevel::Warning, "Error parsing response headers: " << static_cast<int>(res)); // TODO - turn these into strings
	}	
}

} //end ns


