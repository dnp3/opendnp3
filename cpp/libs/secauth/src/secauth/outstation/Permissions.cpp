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

#include "Permissions.h"

using namespace opendnp3;

namespace secauth
{
	
Permissions::Permissions(bool allowByDefault) :
	confirm(allowByDefault),
	read(allowByDefault),
	write(allowByDefault),
	select(allowByDefault),
	operate(allowByDefault),
	directOperate(allowByDefault),
	directOperateNR(allowByDefault),
	immediateFreeze(allowByDefault),
	immediateFreezeNR(allowByDefault),
	freezeClear(allowByDefault),
	freezeClearNR(allowByDefault),
	freezeAtTime(allowByDefault),
	freezeAtTimeNR(allowByDefault),
	coldRestart(allowByDefault),
	warmRestart(allowByDefault),
	initializeData(allowByDefault),
	initializeApplication(allowByDefault),
	startApplication(allowByDefault),
	stopApplication(allowByDefault),
	saveConfiguration(allowByDefault),
	enableUnsolicited(allowByDefault),
	disableUnsolicited(allowByDefault),
	assignClass(allowByDefault),
	delayMeasure(allowByDefault),
	recordCurrentTime(allowByDefault),
	openFile(allowByDefault),
	closeFile(allowByDefault),
	deleteFile(allowByDefault),
	getFileInfo(allowByDefault),
	authenticateFile(allowByDefault),
	abortFile(allowByDefault)
{}

	
Permissions Permissions::AllowNothing()
{
	return Permissions(false);
}

Permissions Permissions::AllowAll()
{
	return Permissions(true);
}
	
void Permissions::Allow(FunctionCode code)
{
	auto perm = GetPermission(code);
	if(perm) {
		perm->allowed = true;
	}
}

void Permissions::Deny(FunctionCode code)
{
	auto perm = GetPermission(code);
	if (perm) {
		perm->allowed = false;
	}
}

bool Permissions::IsAllowed(opendnp3::FunctionCode code) const
{
	// dirty dirty hack, but a private dirty dirty hack
	auto perm = const_cast<Permissions*>(this)->GetPermission(code);
	
	return perm ? perm->allowed : false;	
}

Permissions::Permission* Permissions::GetPermission(FunctionCode code)
{
	switch (code)
	{
		case(FunctionCode::CONFIRM) :
			return &confirm;
		case(FunctionCode::READ) :
			return &read;
		case(FunctionCode::WRITE) :
			return &write;
		case(FunctionCode::SELECT) :
			return &select;
		case(FunctionCode::OPERATE) :
			return &operate;
		case(FunctionCode::DIRECT_OPERATE) :
			return &directOperate;
		case(FunctionCode::DIRECT_OPERATE_NR) :
			return &directOperateNR;
		case(FunctionCode::IMMED_FREEZE) :
			return &immediateFreeze;
		case(FunctionCode::IMMED_FREEZE_NR) :
			return &immediateFreezeNR;
		case(FunctionCode::FREEZE_CLEAR) :
			return &freezeClear;
		case(FunctionCode::FREEZE_CLEAR_NR) :
			return &freezeClearNR;
		case(FunctionCode::FREEZE_AT_TIME) :
			return &freezeAtTime;
		case(FunctionCode::FREEZE_AT_TIME_NR) :
			return &freezeAtTimeNR;
		case(FunctionCode::COLD_RESTART) :
			return &coldRestart;
		case(FunctionCode::WARM_RESTART) :
			return &warmRestart;
		case(FunctionCode::INITIALIZE_DATA) :
			return &initializeData;
		case(FunctionCode::INITIALIZE_APPLICATION) :
			return &initializeApplication;
		case(FunctionCode::START_APPLICATION) :
			return &startApplication;
		case(FunctionCode::STOP_APPLICATION) :
			return &stopApplication;
		case(FunctionCode::SAVE_CONFIGURATION) :
			return &saveConfiguration;
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return &enableUnsolicited;
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return &disableUnsolicited;
		case(FunctionCode::ASSIGN_CLASS) :
			return &assignClass;
		case(FunctionCode::DELAY_MEASURE) :
			return &delayMeasure;
		case(FunctionCode::RECORD_CURRENT_TIME) :
			return &recordCurrentTime;
		case(FunctionCode::OPEN_FILE) :
			return &openFile;
		case(FunctionCode::CLOSE_FILE) :
			return &closeFile;
		case(FunctionCode::DELETE_FILE) :
			return &deleteFile;
		case(FunctionCode::GET_FILE_INFO) :
			return &getFileInfo;
		case(FunctionCode::AUTHENTICATE_FILE) :
			return &authenticateFile;
		case(FunctionCode::ABORT_FILE) :
			return &abortFile;			
		default:
			return nullptr;
	}
}

}



