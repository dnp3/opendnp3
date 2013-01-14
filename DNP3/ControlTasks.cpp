
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
#include "ControlTasks.h"
#include "APDU.h"

#include <APL/Logger.h>
#include <APL/LoggableMacros.h>

namespace apl
{
namespace dnp
{

ControlTaskBase::ControlTaskBase(Logger* apLogger) :
	MasterTaskBase(apLogger),
	mState(INVALID)
{}

bool ControlTaskBase::GetSelectBit()
{
	switch(mState) {
	case(SELECT): return true;
	case(OPERATE): return false;
	default:
		throw InvalidStateException(LOCATION, "INVALID");
	}
}

void ControlTaskBase::Respond(CommandStatus aStatus)
{
	mData.mpRspAcceptor->AcceptResponse(CommandResponse(aStatus), mData.mSequence);
}

void ControlTaskBase::OnFailure()
{
	this->Respond(CS_HARDWARE_ERROR);
}

TaskResult ControlTaskBase::_OnPartialResponse(const APDU& arAPDU)
{
	LOG_BLOCK(LEV_ERROR, "Non fin responses not allowed for control tasks");
	return TR_CONTINUE;
}

TaskResult ControlTaskBase::_OnFinalResponse(const APDU& arAPDU)
{
	CommandStatus cs = mValidator(arAPDU);

	if(mState == SELECT && cs == CS_SUCCESS) {
		mState = OPERATE;
		return TR_CONTINUE;
	}
	else {
		this->Respond(cs);
		return TR_SUCCESS;
	}

}

/* -------- BinaryOutputTask -------- */

BinaryOutputTask::BinaryOutputTask(Logger* apLogger) :
	ControlTask<BinaryOutput>(apLogger)
{}

CommandObject<BinaryOutput>* BinaryOutputTask::GetObject(const BinaryOutput&)
{
	return Group12Var1::Inst();
}

/* -------- SetpointTask -------- */

SetpointTask::SetpointTask(Logger* apLogger) :
	ControlTask<Setpoint>(apLogger)
{}

CommandObject<Setpoint>* SetpointTask::GetOptimalEncoder(SetpointEncodingType aType)
{
	switch(aType) {
	case SPET_INT16: return Group41Var2::Inst();
	case SPET_INT32: return Group41Var1::Inst();
	case SPET_FLOAT: return Group41Var3::Inst();
	case SPET_DOUBLE: return Group41Var4::Inst();
	default:
		throw ArgumentException(LOCATION, "Enum not handled");
	}
}

CommandObject<Setpoint>* SetpointTask::GetObject(const Setpoint& arSetpoint)
{
	return GetOptimalEncoder(arSetpoint.GetOptimalEncodingType());
}



}
} //ens ns
