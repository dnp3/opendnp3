
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
#ifndef __CONTROL_TERMINAL_EXTENSION_H_
#define __CONTROL_TERMINAL_EXTENSION_H_

#include "TerminalInterfaces.h"

#include <APL/DataInterfaces.h>
#include <APL/CommandResponseQueue.h>

#include <string>

namespace apl
{
class ICommandAcceptor;


class ControlTerminalExtension : public ITerminalExtension
{
public:

	ControlTerminalExtension(ICommandAcceptor* apCmdAcceptor) :
		mSequence(0),
		mpCmdAcceptor(apCmdAcceptor),
		mRspQueue() {

	}

	std::string Name() {
		return "ControlTerminalExtension";
	}

	virtual ~ControlTerminalExtension() {}

	static ControlCode ParseControlCode( const std::string& arString );

private:

	int mSequence;

	ICommandAcceptor* mpCmdAcceptor;
	CommandResponseQueue mRspQueue;

	void WaitForResponse();
	retcode HandleIssueBO(std::vector<std::string>& arArgs);
	retcode HandleIssueST(std::vector<std::string>& arArgs);

	//implement from ITerminalExtension
	void _BindToTerminal(ITerminal* apTerminal);


};
}

#endif
