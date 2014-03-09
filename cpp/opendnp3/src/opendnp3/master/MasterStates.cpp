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
#include "MasterStates.h"



#include "AsyncTaskInterfaces.h"
#include "AsyncTaskGroup.h"
#include "Master.h"

#include <openpal/LoggableMacros.h>

using namespace openpal;

namespace opendnp3
{

/* AMS_Base */

void AMS_Base::StartTask(Master* c, ITask*, MasterTaskBase*)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnLowerLayerUp(Master* c)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnLowerLayerDown(Master* c)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnSendSuccess(Master* c)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnFailure(Master* c)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnPartialResponse(Master* c, const APDUResponseRecord&)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnFinalResponse(Master* c, const APDUResponseRecord&)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}

void AMS_Base::OnUnsolResponse(Master* c, const APDUResponseRecord&)
{
	LOGGER_BLOCK(c->mLogger, LogLevel::Error, "Invalid action for state " << this->Name());
}


void AMS_Base::ChangeState(Master* c, AMS_Base* apState)
{
	c->mpState = apState;
}

void AMS_Base::ChangeTask(Master* c, MasterTaskBase* apTask)
{
	c->mpTask = apTask;
}

/* AMS_Closed */

AMS_Closed AMS_Closed::mInstance;

void AMS_Closed::OnLowerLayerUp(Master* c)
{
	ChangeState(c, AMS_Idle::Inst());
}

/* AMS_OpenBase */

void AMS_OpenBase::OnUnsolResponse(Master* c, const APDUResponseRecord& aRecord)
{
	c->ProcessIIN(aRecord.IIN);
	c->ProcessDataResponse(aRecord);
}

void AMS_OpenBase::OnLowerLayerDown(Master* c)
{
	ChangeState(c, AMS_Closed::Inst());
}

/* AMS_Idle */

AMS_Idle AMS_Idle::mInstance;

void AMS_Idle::StartTask(Master* c, ITask* apScheTask, MasterTaskBase* apMasterTask)
{
	this->ChangeState(c, AMS_Waiting::Inst());
	this->ChangeTask(c, apMasterTask);
	c->mpScheduledTask = apScheTask;
	c->StartTask(apMasterTask, true);
}


/* AMS_WaitForSimpleRsp */

AMS_Waiting AMS_Waiting::mInstance;

void AMS_Waiting::OnLowerLayerDown(Master* c)
{
	ChangeState(c, AMS_Closed::Inst());
	c->mpTask->OnFailure();
}

void AMS_Waiting::OnFailure(Master* c)
{
	this->ChangeState(c, AMS_Idle::Inst());
	c->mpTask->OnFailure();
	c->mpScheduledTask->OnComplete(false);
}

void AMS_Waiting::OnPartialResponse(Master* c, const APDUResponseRecord& aRecord)
{
	c->ProcessIIN(aRecord.IIN);

	if(!c->mpTask->OnPartialResponse(aRecord))
	{
		this->ChangeState(c, AMS_Idle::Inst());
		c->mpScheduledTask->OnComplete(false);
	}
}

void AMS_Waiting::OnFinalResponse(Master* c, const APDUResponseRecord& aRecord)
{
	c->ProcessIIN(aRecord.IIN);

	switch(c->mpTask->OnFinalResponse(aRecord))
	{
	case(TR_FAIL):
		this->ChangeState(c, AMS_Idle::Inst());
		c->mpScheduledTask->OnComplete(false);
		break;
	case(TR_CONTINUE):	//multi request task!
		c->StartTask(c->mpTask, false);
		break;
	case(TR_SUCCESS):
		this->ChangeState(c, AMS_Idle::Inst());
		c->mpScheduledTask->OnComplete(true);
	}


}

} //ens ns

