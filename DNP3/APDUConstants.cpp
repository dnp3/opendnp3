//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "APDUConstants.h"


namespace apl
{
namespace dnp
{

bool IsResponse(FunctionCodes aCode)
{
	return aCode == FC_RESPONSE || aCode == FC_UNSOLICITED_RESPONSE;
}

bool IsRequest(FunctionCodes aCode)
{
	switch(aCode) {
	case(FC_CONFIRM):
	case(FC_RESPONSE):
	case(FC_UNSOLICITED_RESPONSE):
		return false;
	default:
		return true;
	}
}

std::string ToString(FunctionCodes aCode)
{
	switch(aCode) {
	case(FC_CONFIRM) : return "Confirm";
	case(FC_READ): return "Read";
	case(FC_WRITE): return "Write";
	case(FC_SELECT): return "Select";
	case(FC_OPERATE): return "Operate";
	case(FC_DIRECT_OPERATE): return "Direct Operate";
	case(FC_DIRECT_OPERATE_NO_ACK): return "Operate No ACK";

	case(FC_FREEZE): return "Freeze";
	case(FC_FREEZE_NO_ACK): return "Freeze No ACK";
	case(FC_FREEZE_CLEAR): return "Freeze Clear";
	case(FC_FREEZE_CLEAR_NO_ACK): return "Freeze Clear No ACK";
	case(FC_FREEZE_AT_TIME): return "Freeze At Time";
	case(FC_FREEZE_AT_TIME_NO_ACK): return "Freeze At Time No ACK";

	case(FC_COLD_RESTART): return "Cold Restart";
	case(FC_WARM_RESTART): return "Warm Restart";
	case(FC_INITIALIZE_DATA): return "Init Data";
	case(FC_INITIALIZE_APPLICATION): return "Init App";
	case(FC_START_APPLICATION): return "Start App";
	case(FC_STOP_APPLICATION): return "Stop Appl";
	case(FC_SAVE_CONFIGURATION): return "Save Config";
	case(FC_ENABLE_UNSOLICITED): return "Enable Unsol";
	case(FC_DISABLE_UNSOLICITED): return "Disable Unsol";
	case(FC_ASSIGN_CLASS): return "Assign Classs";
	case(FC_DELAY_MEASURE): return "Delay Measure";
	case(FC_RECORD_TIME): return "Record Time";

	case(FC_FILE_OPEN): return "File Open";
	case(FC_FILE_CLOSE): return "File Close";
	case(FC_FILE_DELETE): return "File Delete";
	case(FC_FILE_INFO): return "File Info";
	case(FC_FILE_AUTHENTICATE): return "File Authenticate";
	case(FC_FILE_ABORT): return "File Abort";

	case(FC_RESPONSE): return "Rsp";
	case(FC_UNSOLICITED_RESPONSE): return "Unsol Rsp";

	default: return "Undefined";
	}
}

}
}
