
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

#include "CommandTask.h"
#include "LoggableMacros.h"

#include <opendnp3/Logger.h>
#include <opendnp3/Exception.h>



namespace opendnp3
{

CommandTask::CommandTask(Logger* apLogger) : MasterTaskBase(apLogger)
{

}

void CommandTask::Configure(const Formatter& arFormatter, const Responder& arResponder)
{
	mFormatter = arFormatter;
	mResponder = arResponder;
	mCodes.clear();
}

void CommandTask::AddCommandCode(FunctionCodes aCode)
{
	this->mCodes.push_back(aCode);
}

void CommandTask::ConfigureRequest(APDU& arAPDU)
{
	if(mCodes.empty()) MACRO_THROW_EXCEPTION(InvalidStateException, "No more functions in sequence");
	mValidator = mFormatter(arAPDU, mCodes.front());
	mCodes.pop_front();
}

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string CommandTask::Name() const
{
	return "CommandTask";
}
#endif

void CommandTask::OnFailure()
{
	mResponder(CS_HARDWARE_ERROR);
}

TaskResult CommandTask::_OnPartialResponse(const APDU& arAPDU)
{
	LOG_BLOCK(LEV_ERROR, "Non fin responses not allowed for control tasks");
	return TR_CONTINUE;
}

TaskResult CommandTask::_OnFinalResponse(const APDU& arAPDU)
{
	CommandStatus cs = mValidator(arAPDU);
	if(cs == CS_SUCCESS) {
		if(mCodes.empty()) {
			mResponder(cs);
			return TR_SUCCESS;
		}
		else return TR_CONTINUE;
	}
	else {
		mResponder(cs);
		return TR_SUCCESS;
	}
}

} //ens ns


