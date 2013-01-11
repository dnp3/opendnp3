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
#ifndef __COMMAND_RESPONSE_QUEUE_H_
#define __COMMAND_RESPONSE_QUEUE_H_

#include "CommandInterfaces.h"
#include "INotifier.h"
#include "SubjectBase.h"

#include <queue>
#include <mutex>
#include <condition_variable>

namespace apl
{
class CommandResponseQueue : public IResponseAcceptor, public SubjectBase<std::mutex>
{
public:
	CommandResponseQueue();

	void AcceptResponse(const CommandResponse&, int aSequence);
	bool WaitForResponse(CommandResponse&, int, millis_t aTimeout = -1);

private:

	bool FindResponse(int aSeq, CommandResponse& arRsp);

	CommandResponse cr;
	int mSequence;
	std::mutex mMutex;
	std::condition_variable mCondition;

	struct RspInfo {
		RspInfo(const CommandResponse& arRsp, int aSequence) : mResponse(arRsp), mSequence(aSequence) {}
		CommandResponse mResponse; int mSequence;
	};

	std::deque<RspInfo> mResponseQueue;
};
}
#endif
