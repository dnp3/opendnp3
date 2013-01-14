
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
#ifndef __MOCK_COMMAND_ACCEPTOR_H_
#define __MOCK_COMMAND_ACCEPTOR_H_

#include <APL/CommandInterfaces.h>
#include <APL/Exception.h>
#include <APL/Location.h>

#include <queue>

namespace apl
{

class MockCommandAcceptor : public ICommandAcceptor
{
public:

	void AcceptCommand(const BinaryOutput& aBo, size_t, int aSequence, IResponseAcceptor* apRspAcceptor) {
		mBinaryOutputs.push(aBo);
		this->AcceptCommand(aSequence, apRspAcceptor);
	}

	void AcceptCommand(const Setpoint& aSt, size_t, int aSequence, IResponseAcceptor* apRspAcceptor) {
		mSetpoints.push(aSt);
		this->AcceptCommand(aSequence, apRspAcceptor);
	}

	void Queue(CommandStatus aStatus) {
		mResponses.push(aStatus);
	}

	Setpoint NextSetpoint() {
		if(mSetpoints.size() == 0) throw Exception(LOCATION, "no setpoints recieved");
		Setpoint s = mSetpoints.front();
		mSetpoints.pop();
		return s;
	}
	BinaryOutput NextBinaryOutput() {
		if(mBinaryOutputs.size() == 0) throw Exception(LOCATION, "no binary outputs recieved");
		BinaryOutput b = mBinaryOutputs.front();
		mBinaryOutputs.pop();
		return b;
	}

private:

	void AcceptCommand(int aSeq, IResponseAcceptor* apRspAcceptor) {
		if(mResponses.empty()) throw Exception(LOCATION, "response queue is empty");
		else {
			CommandResponse rsp(mResponses.front());
			mResponses.pop();
			apRspAcceptor->AcceptResponse(rsp, aSeq);
		}

	}

	std::queue<CommandStatus> mResponses;
	std::queue<Setpoint> mSetpoints;
	std::queue<BinaryOutput> mBinaryOutputs;
};

}

#endif
