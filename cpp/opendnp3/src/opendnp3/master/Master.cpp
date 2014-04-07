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

#include "opendnp3/master/MasterStates.h"
#include "opendnp3/master/MeasurementHandler.h"
#include "opendnp3/master/ConstantCommandProcessor.h"
#include "opendnp3/master/AsyncTaskInterfaces.h"
#include "opendnp3/master/AsyncTaskGroup.h"
#include "opendnp3/master/AsyncTaskBase.h"
#include "opendnp3/master/AsyncTaskPeriodic.h"
#include "opendnp3/master/AsyncTaskNonPeriodic.h"
#include "opendnp3/master/AsyncTaskContinuous.h"

#include "opendnp3/app/APDUParser.h"
#include "opendnp3/app/APDURequest.h"

#include "opendnp3/LogLevels.h"

#include <openpal/IExecutor.h>

#include <openpal/LogMacros.h>

using namespace openpal;

namespace opendnp3
{

Master::Master(LogRoot& root, MasterConfig aCfg, IAppLayer* apAppLayer, ISOEHandler* apSOEHandler, AsyncTaskGroup* apTaskGroup, openpal::IExecutor* apExecutor, IUTCTimeSource* apTimeSrc) :
	IAppUser(root),
	pExecutor(apExecutor),
	mpAppLayer(apAppLayer),
	mpSOEHandler(apSOEHandler),
	mpTaskGroup(apTaskGroup),
	mpTimeSrc(apTimeSrc),
	mpState(AMS_Closed::Inst()),
	mpTask(nullptr),
	mpScheduledTask(nullptr),
	mSchedule(apTaskGroup, this, aCfg),
	mClassPoll(logger, apSOEHandler),
	mClearRestart(logger),
	mConfigureUnsol(logger),
	mTimeSync(logger, apTimeSrc),
	mCommandTask(logger),
	mCommandQueue(apExecutor, mSchedule.mpCommandTask)
{

}

void Master::ProcessIIN(const IINField& iin)
{
	mLastIIN = iin;

	bool check_state = false;

	//The clear IIN task only happens in response to detecting an IIN bit.
	if(mLastIIN.IsSet(IINBit::NEED_TIME))
	{
		//LOG_BLOCK(flags::INFO, "Need time detected");
		mSchedule.mpTimeTask->SilentEnable();
		check_state = true;
	}

	if (mLastIIN.IsSet(IINBit::DEVICE_TROUBLE))
	{
		//LOG_BLOCK(flags::WARN, "IIN Device trouble detected");
	}

	if (mLastIIN.IsSet(IINBit::EVENT_BUFFER_OVERFLOW))
	{
		//LOG_BLOCK(flags::WARN, "Event buffer overflow detected");
	}

	// If this is detected, we need to reset the startup tasks
	if(mLastIIN.IsSet(IINBit::DEVICE_RESTART))
	{
		//LOG_BLOCK(flags::WARN, "Device restart detected");
		mSchedule.ResetStartupTasks();
		mSchedule.mpClearRestartTask->SilentEnable();
		check_state = true;
	}

	if(check_state) mpTaskGroup->CheckState();
}

void Master::ProcessCommand(ITask* apTask)
{
	if(mpState == AMS_Closed::Inst())   //we're closed
	{
		ConstantCommandProcessor ccp(pExecutor, CommandResponse(CommandResult::NO_COMMS));
		while(mCommandQueue.Dispatch(&ccp));
		apTask->Disable();
	}
	else
	{
		if(mCommandQueue.Dispatch(this))
		{
			mpState->StartTask(this, apTask, &mCommandTask);
		}
		else apTask->Disable();
	}
}

void Master::SelectAndOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureSBO(command, index, pCallback);
}

void Master::SelectAndOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureSBO(command, index, pCallback);
}

void Master::SelectAndOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureSBO(command, index, pCallback);
}

void Master::SelectAndOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureSBO(command, index, pCallback);
}

void Master::SelectAndOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureSBO(command, index, pCallback);
}

void Master::DirectOperate(const ControlRelayOutputBlock& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureDO(command, index, pCallback);
}

void Master::DirectOperate(const AnalogOutputInt32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureDO(command, index, pCallback);
}

void Master::DirectOperate(const AnalogOutputInt16& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureDO(command, index, pCallback);
}

void Master::DirectOperate(const AnalogOutputFloat32& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureDO(command, index, pCallback);
}

void Master::DirectOperate(const AnalogOutputDouble64& command, uint16_t index, ICommandCallback* pCallback)
{
	this->mCommandTask.ConfigureDO(command, index, pCallback);
}

void Master::StartTask(MasterTaskBase* apMasterTask, bool aInit)
{
	if(aInit) apMasterTask->Init();
	APDURequest request(this->requestBuffer.GetWriteBuffer());
	request.SetControl(AppControlField(true, true, false, false));
	apMasterTask->ConfigureRequest(request);
	mpAppLayer->SendRequest(request);
}


/* Tasks */

void Master::SyncTime(ITask* apTask)
{
	if(mLastIIN.IsSet(IINBit::NEED_TIME))
	{
		mpState->StartTask(this, apTask, &mTimeSync);
	}
	else apTask->Disable();
}

void Master::WriteIIN(ITask* apTask)
{
	if(mLastIIN.IsSet(IINBit::DEVICE_RESTART))
	{
		mpState->StartTask(this, apTask, &mClearRestart);
	}
	else apTask->Disable();
}

void Master::IntegrityPoll(ITask* apTask)
{
	mClassPoll.Set(CLASS_1 | CLASS_2 | CLASS_3 | CLASS_0);
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
	return MasterScan(pExecutor, mSchedule.mpIntegrityPoll);
}

MasterScan Master::AddClassScan(int aClassMask, openpal::TimeDuration aScanRate, openpal::TimeDuration aRetryRate)
{
	auto pTask = mSchedule.AddClassScan(aClassMask, aScanRate, aRetryRate);
	return MasterScan(pExecutor, pTask);
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
}

void Master::OnSolSendSuccess()
{
	mpState->OnSendSuccess(this);
}

void Master::OnSolFailure()
{
	mpState->OnFailure(this);
}

void Master::OnUnsolSendSuccess()
{
	//LOG_BLOCK(flags::ERR, "Master can't send unsol");
}

void Master::OnUnsolFailure()
{
	//LOG_BLOCK(flags::ERR, "Master can't send unsol");
}

void Master::OnPartialResponse(const APDUResponseRecord& aRecord)
{
	mpState->OnPartialResponse(this, aRecord);
}

void Master::OnFinalResponse(const APDUResponseRecord& aRecord)
{
	mpState->OnFinalResponse(this, aRecord);
}

void Master::OnUnsolResponse(const APDUResponseRecord& aRecord)
{
	mpState->OnUnsolResponse(this, aRecord);
}

/* Private functions */

void Master::ProcessDataResponse(const APDUResponseRecord& record)
{
	MeasurementHandler handler(logger, this->mpSOEHandler);
	APDUParser::ParseTwoPass(record.objects, &handler, &logger);
}

} //end ns


