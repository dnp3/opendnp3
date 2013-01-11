
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
#include "DOTerminalExtension.h"

#include <APL/Util.h>
#include <APL/Exception.h>
#include <APL/Parsing.h>

#include <sstream>
#include <functional>

using namespace std;
using namespace std::placeholders;

namespace apl
{
void DOTerminalExtension::_BindToTerminal(ITerminal* apTerminal)
{
	CommandNode cmd;

	ostringstream oss;


	cmd.mName = "bi";
	oss << "queue bi <index> <0|1> <quality: #|" << Binary::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues a binary input value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueBinary, this, _1);
	apTerminal->BindCommand(cmd, "queue bi");

	cmd.mName = "ai";
	oss.str("");
	oss << "queue ai <index> <#.#> <quality: #|" << Analog::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues an analog input value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueAnalog, this, _1);
	apTerminal->BindCommand(cmd, "queue ai");

	cmd.mName = "c";
	oss.str("");
	oss << "queue c <index> <0|1> <quality: #|" << Counter::QualConverter::GetAllSymbols() << ">";
	cmd.mUsage = oss.str();
	cmd.mDesc = "Queues an counter value into the transaction buffer";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleQueueCounter, this, _1);
	apTerminal->BindCommand(cmd, "queue c");

	cmd.mName = "flush";
	cmd.mUsage = "flush";
	cmd.mDesc = "Flushes the output queues to the data observer (an optional number of times).";
	cmd.mHandler = std::bind(&DOTerminalExtension::HandleDoTransaction, this, _1);
	apTerminal->BindCommand(cmd, "flush");
}

retcode DOTerminalExtension::HandleDoTransaction(std::vector<std::string>& arArgs)
{
	if(arArgs.size() > 0) return BAD_ARGUMENTS;
	mBuffer.FlushUpdates(mpObserver);
	return SUCCESS;
}

}

