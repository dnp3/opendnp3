
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
#ifndef __CONTROL_RESPONSE_TERMINAL_EXTENSION_H_
#define __CONTROL_RESPONSE_TERMINAL_EXTENSION_H_

#include <APL/Loggable.h>
#include <APL/DataInterfaces.h>
#include <APL/CommandInterfaces.h>

#include "TerminalInterfaces.h"

#include <mutex>

namespace apl
{

/** Terminal extension that allows you to set the response to controlsor setpoints from a master.
*/
class CommandResponder : public ICommandAcceptor, public Loggable
{
public:

	CommandResponder(Logger* apLogger, bool aLinkStatuses = false, IDataObserver* apObs = NULL);

	void AcceptCommand(const BinaryOutput&, size_t, int aSequence, IResponseAcceptor* apRspAcceptor);
	void AcceptCommand(const Setpoint&, size_t, int aSequence, IResponseAcceptor* apRspAcceptor);

	CommandStatus HandleControl(const BinaryOutput& aControl, size_t aIndex);
	CommandStatus HandleControl(const Setpoint& aControl, size_t aIndex);

	void SetResponseCode(bool aType, size_t aIndex, CommandStatus aCode);

	CommandStatus GetResponseCode(bool aType, size_t aIndex);

private:

	std::mutex mMutex;
	typedef std::map<size_t, CommandStatus> CommandMap;
	CommandMap mBinaryResponses;
	CommandMap mSetpointResponses;

	ICommandSource* mpSource;
	IDataObserver* mpObs;
	bool mLinkStatuses;
};

class ControlResponseTE : public ITerminalExtension
{
public:

	ControlResponseTE(Logger* apLogger, bool aLinkStatuses = false, IDataObserver* apObs = NULL);

	std::string Name() {
		return "ControlResponseTE";
	}
	ICommandAcceptor* GetCmdAcceptor() {
		return &mHandler;
	}

private:

	void _BindToTerminal(ITerminal* apTerminal);

	retcode HandleSetResponse(std::vector<std::string>& arArgs);

	CommandResponder mHandler;
};
}

#endif
