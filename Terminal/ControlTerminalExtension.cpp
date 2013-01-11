
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
#include "ControlTerminalExtension.h"

#include <APL/FlexibleDataObserver.h>
#include <APL/CommandInterfaces.h>
#include <APL/CommandTypes.h>
#include <APL/CommandResponseQueue.h>
#include <APL/Util.h>
#include <APL/Parsing.h>

#include <functional>
#include <sstream>

using namespace std;
using namespace std::placeholders;

namespace apl
{
typedef std::function<std::string (uint8_t)> QualityFunctor;

void ControlTerminalExtension::_BindToTerminal(ITerminal* apTerminal)
{
	CommandNode cmd;

	cmd.mName = "issue bo";
	cmd.mUsage  = "issue bo <index> lon|loff\r\n";
	cmd.mUsage += "       issue bo <index> pon|pclose|ptrip [<on time>] [<off time>] [<count>]";
	cmd.mDesc = "Issues a binary output command. ";
	cmd.mHandler = std::bind(&ControlTerminalExtension::HandleIssueBO, this, _1);
	apTerminal->BindCommand(cmd, "issue bo");

	cmd.mName = "issue st";
	cmd.mUsage = "issue st <index> <integer|double>";
	cmd.mDesc = "Issues a setpoint request. If the value contains a \'.\', it is treated as a double.";
	cmd.mHandler = std::bind(&ControlTerminalExtension::HandleIssueST, this, _1);
	apTerminal->BindCommand(cmd, "issue st");
}

void ControlTerminalExtension::WaitForResponse()
{
	this->Send("Waiting for response... ");
	CommandResponse rsp;
	bool success = mRspQueue.WaitForResponse(rsp, mSequence, 5000);
	ostringstream oss;
	if(success) {
		oss << "Result: " << ToString(rsp.mResult) << ITerminal::EOL;
		this->Send(oss.str());
	}
	else {
		oss << "Timeout" << ITerminal::EOL;
		this->Send(oss.str());
	}
}


retcode ControlTerminalExtension::HandleIssueST(std::vector<std::string>& arArgs)
{
	if(arArgs.size() < 2) return BAD_ARGUMENTS;

	Setpoint st;

	int index;

	if(!Parsing::GetPositive(arArgs[0], index)) return BAD_ARGUMENTS;
	if(arArgs[1].find('.') == std::string::npos) {
		int iValue;
		if(!Parsing::Get(arArgs[1], iValue)) return BAD_ARGUMENTS;
		st.SetValue(static_cast<int32_t>(iValue));
	}
	else {
		double dValue;
		if(!Parsing::Get(arArgs[1], dValue)) return BAD_ARGUMENTS;
		st.SetValue(dValue);
	}

	mpCmdAcceptor->AcceptCommand(st, static_cast<size_t>(index), ++mSequence, &mRspQueue);
	WaitForResponse();
	return SUCCESS;
}

ControlCode ControlTerminalExtension::ParseControlCode( const std::string& arString )
{
	std::string lower(arString);
	toLowerCase(lower);
	if ( lower.compare("pon") == 0 )			return CC_PULSE;
	else if ( lower.compare("lon") == 0 )		return CC_LATCH_ON;
	else if ( lower.compare("loff") == 0 )		return CC_LATCH_OFF;
	else if ( lower.compare("pclose") == 0 )	return CC_PULSE_CLOSE;
	else if ( lower.compare("ptrip") == 0 )		return CC_PULSE_TRIP;
	else return CC_UNDEFINED;
}

retcode ControlTerminalExtension::HandleIssueBO(std::vector<std::string>& arArgs)
{
	if(arArgs.size() < 2) return BAD_ARGUMENTS;

	BinaryOutput b; b.mOnTimeMS = 100; b.mOffTimeMS = 100; b.mCount = 1;
	uint32_t index;
	if(!Parsing::Get(arArgs[0], index)) return BAD_ARGUMENTS;

	b.mRawCode = static_cast<uint8_t>(ParseControlCode(arArgs[1]));
	switch(b.mRawCode) {
	case(CC_PULSE):
	case(CC_PULSE_CLOSE):
	case(CC_PULSE_TRIP):
		switch(arArgs.size()) {
		case(5):
			if(!Parsing::Get(arArgs[4], b.mCount)) return BAD_ARGUMENTS;
		case(4):
			if(!Parsing::Get(arArgs[3], b.mOffTimeMS)) return BAD_ARGUMENTS;
		case(3):
			if(!Parsing::Get(arArgs[2], b.mOnTimeMS)) return BAD_ARGUMENTS;
		case(2):
			break;
		default:
			return BAD_ARGUMENTS;
		}
		break;
	case(CC_LATCH_ON):
	case(CC_LATCH_OFF):
		if(arArgs.size() > 2) return BAD_ARGUMENTS;
		break;
	default:
		return BAD_ARGUMENTS;
	}

	mpCmdAcceptor->AcceptCommand(b, index, ++mSequence, &mRspQueue);
	WaitForResponse();
	return SUCCESS;
}

}

