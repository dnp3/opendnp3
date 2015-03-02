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

#include "CriticalFunctions.h"

using namespace opendnp3;

namespace secauthv5
{

CriticalFunctions CriticalFunctions::AuthOptional()
{
	return CriticalFunctions(false);
}

CriticalFunctions CriticalFunctions::AuthEverything()
{
	return CriticalFunctions(true);
}

CriticalFunctions::CriticalFunctions(bool authOptionalCodes) :
	authConfirm(authOptionalCodes),
	authRead(authOptionalCodes),
	authImmediateFreeze(authOptionalCodes),
	authImmediateFreezeNR(authOptionalCodes),
	authFreezeClear(authOptionalCodes),
	authFreezeClearNR(authOptionalCodes),
	authFreezeAtTime(authOptionalCodes),
	authFreezeAtTimeNR(authOptionalCodes),
	authInitData(authOptionalCodes),
	authAssignClass(authOptionalCodes),
	authDelayMeasure(authOptionalCodes),
	authResponse(authOptionalCodes),
	authUnsolicited(authOptionalCodes)
{}

bool CriticalFunctions::IsCritical(FunctionCode code) const
{
	switch (code)
	{
	case(FunctionCode::CONFIRM) :
		return authConfirm;
	case(FunctionCode::READ) :
		return authRead;
	case(FunctionCode::IMMED_FREEZE) :
		return authImmediateFreeze;
	case(FunctionCode::IMMED_FREEZE_NR):
		return authImmediateFreezeNR;
	case(FunctionCode::FREEZE_CLEAR) :
		return authFreezeClear;
	case(FunctionCode::FREEZE_CLEAR_NR) :
		return authFreezeClearNR;
	case(FunctionCode::FREEZE_AT_TIME) :
		return authFreezeAtTime;
	case(FunctionCode::FREEZE_AT_TIME_NR) :
		return authFreezeAtTimeNR;
	case(FunctionCode::INITIALIZE_DATA) :
		return authInitData;
	case(FunctionCode::ASSIGN_CLASS) :
		return authAssignClass;
	case(FunctionCode::DELAY_MEASURE) :
		return authDelayMeasure;
	case(FunctionCode::RESPONSE) :
		return authResponse;
	case(FunctionCode::UNSOLICITED_RESPONSE) :
		return authUnsolicited;
	default:
		return true;
	}
}

}

