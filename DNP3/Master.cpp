
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
#include "Master.h"

#include "MasterStates.h"
#include "ObjectReadIterator.h"
#include "ResponseLoader.h"
#include "VtoEventBufferAdapter.h"
#include "ConstantCommandProcessor.h"
#include "CommandHelpers.h"

#include <APL/DataInterfaces.h>
#include <APL/AsyncTaskInterfaces.h>
#include <APL/AsyncTaskGroup.h>
#include <APL/AsyncTaskBase.h>
#include <APL/AsyncTaskPeriodic.h>
#include <APL/AsyncTaskNonPeriodic.h>
#include <APL/Logger.h>
#include <APL/IExecutor.h>
#include <APL/AsyncTaskContinuous.h>
#include <APL/CopyableBuffer.h>

#include <functional>

namespace apl
{
namespace dnp
{

Master::Master(Logger* apLogger, MasterConfig aCfg, IAppLayer* apAppLayer, IDataObserver* apPublisher, AsyncTaskGroup* apTaskGroup, IExecutor* apExecutor, ITimeSource* apTimeSrc) :
	Loggable(apLogger),
	mVtoReader(apLogger),
	mVtoWriter(apLogger->GetSubLogger("VtoWriter"), aCfg.VtoWriterQueueSize),
	mRequest(aCfg.FragSize),
	mpAppLayer(apAppLayer),
	mpPublisher(apPublisher),
	mpTaskGroup(apTaskGroup),
	mpExecutor(apExecutor),
	mpTimeSrc(apTimeSrc),
	mpState(AMS_Closed::Inst()),
	mpTask(NULL),
	mpScheduledTask(NULL),
	mpObserver(aCfg.mpObserver),
	mState(SS_UNKNOWN),
	mSchedule(apTaskGroup, this, aCfg),
	mClassPoll(apLogger, apPublisher, &mVtoReader),
	mClearRestart(apLogger),
	mConfigureUnsol(apLogger),
	mTimeSync(apLogger, apTimeSrc),
	mCommandTask(apLogger),
	mVtoTransmitTask(apLogger, aCfg.FragSize, aCfg.UseNonStandardVtoFunction)
{
	/*
	 * Establish a link between the mCommandQueue and the
	 * mSchedule.mpCommandTask.  When new data is written to mCommandQueue,
	 * wake up mpCommandTask to process the data.
	 */
	mCommandQueue.AddObserver(mpExecutor, [this](){ 
		this->mSchedule.mpCommandTask->Enable();
	});

	/*
	 * Establish a link between the mVtoWriter and the
	 * mSchedule.mpVtoTransmitTask.  When new data is written to
	 * mVtoWriter, wake up the mSchedule.mpVtoTransmitTask.
	 */
	mVtoWriter.AddObserver(mpExecutor, [this](){ 
		this->mSchedule.mpVtoTransmitTask->Enable(); 
	});

	/*
	 * Set the initial state of the communication link.
	 */
	this->UpdateState(SS_COMMS_DOWN);
}

void Master::UpdateState(StackStates aState)
{
	if(mState != aState) {
		LOG_BLOCK(LEV_INFO, "StackState: " << ConvertStackStateToString(aState));
		mState = aState;
		if(mpObserver != NULL) mpObserver->OnStateChange(aState);
		if(mState == SS_COMMS_UP) {
			mSchedule.mpVtoTransmitTask->Enable();
		}
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
		ConstantCommandProcessor ccp(mpExecutor, CS_HARDWARE_ERROR);			
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

void Master::Select(const BinaryOutput& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureBinaryOutputTask(FC_SELECT, arCommand, aIndex, aCallback);
}

void Master::Select(const Setpoint& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureSetpointTask(FC_SELECT, arCommand, aIndex, aCallback);
}

void Master::Operate(const BinaryOutput& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureBinaryOutputTask(FC_OPERATE, arCommand, aIndex, aCallback);
}

void Master::Operate(const Setpoint& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureSetpointTask(FC_OPERATE, arCommand, aIndex, aCallback);
}

void Master::DirectOperate(const BinaryOutput& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureBinaryOutputTask(FC_DIRECT_OPERATE, arCommand, aIndex, aCallback);
}


void Master::DirectOperate(const Setpoint& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	this->ConfigureSetpointTask(FC_DIRECT_OPERATE, arCommand, aIndex, aCallback);
}

void Master::ConfigureBinaryOutputTask(FunctionCodes aCode, const BinaryOutput& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	auto formatter = [=](APDU& arAPDU){ 
		return CommandHelpers::ConfigureRequest(arAPDU, aCode, arCommand, aIndex, Group12Var1::Inst());
	};
	auto responder = [=](CommandStatus aStatus){
		mpExecutor->Post([=](){ 
			aCallback(CommandResponse(aStatus));
		});
	};
	mCommandTask.Configure(formatter, responder);
}

void Master::ConfigureSetpointTask(FunctionCodes aCode, const Setpoint& arCommand, size_t aIndex, std::function<void (CommandResponse)> aCallback)
{
	auto formatter = [=](APDU& arAPDU){ 
		auto pObj = CommandHelpers::GetOptimalEncoder(arCommand.GetOptimalEncodingType());
		return CommandHelpers::ConfigureRequest(arAPDU, aCode, arCommand, aIndex, pObj);
	};
	auto responder = [=](CommandStatus aStatus){
		mpExecutor->Post([=](){ 
			aCallback(CommandResponse(aStatus));
		});
	};
	mCommandTask.Configure(formatter, responder);
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

void Master::TransmitVtoData(ITask* apTask)
{
	if(mpState == AMS_Closed::Inst()) apTask->Disable();
	else {
		size_t max = mVtoTransmitTask.mBuffer.NumAvailable();
		VtoEventBufferAdapter adapter(&mVtoTransmitTask.mBuffer);
		mVtoWriter.Flush(&adapter, max);

		LOG_BLOCK(LEV_DEBUG, "TransmitVtoData: " << std::boolalpha << mVtoTransmitTask.mBuffer.IsFull() << " size: " << mVtoTransmitTask.mBuffer.Size());

		/* Any data to transmit? */
		if (mVtoTransmitTask.mBuffer.Size() > 0) {
			/* Start the mVtoTransmitTask */
			mpState->StartTask(this, apTask, &mVtoTransmitTask);
		}
		else {
			/* Stop the mVtoTransmitTask */
			apTask->Disable();
		}
	}
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
	throw InvalidStateException(LOCATION, "Master can't send unsol");
}

void Master::OnUnsolFailure()
{
	throw InvalidStateException(LOCATION, "Master can't send unsol");
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
		ResponseLoader loader(this->mpLogger, this->mpPublisher, this->GetVtoReader());

		for(HeaderReadIterator hdr = arResponse.BeginRead(); !hdr.IsEnd(); ++hdr)
			loader.Process(hdr);
	}
	catch(Exception ex) {
		EXCEPTION_BLOCK(LEV_WARNING, ex)
	}
}

}
} //end ns

/* vim: set ts=4 sw=4: */
