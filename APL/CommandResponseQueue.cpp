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
#include "CommandResponseQueue.h"

#include <assert.h>


namespace apl
{

CommandResponseQueue::CommandResponseQueue() {}

void CommandResponseQueue::AcceptResponse(const CommandResponse& arRsp, int aSequence)
{
	{
		std::unique_lock<std::mutex> lock(mMutex);
		mResponseQueue.push_front(RspInfo(arRsp, aSequence));
		mCondition.notify_one();
	}
	this->NotifyAll();
}

bool CommandResponseQueue::WaitForResponse(CommandResponse& arRsp, int aSeq, millis_t aTimeout)
{
	std::unique_lock<std::mutex> lock(mMutex);

	//first try to fun the response, maybe it already got put in before we waited
	if(FindResponse(aSeq, arRsp)) return true;

	if(aTimeout < 0) mCondition.wait(lock);
	else mCondition.wait_for(lock, std::chrono::milliseconds(aTimeout));

	return FindResponse(aSeq, arRsp);
}

bool CommandResponseQueue::FindResponse(int aSeq, CommandResponse& arRsp)
{
	while(mResponseQueue.size() > 0) {
		RspInfo rsp = mResponseQueue.front();
		mResponseQueue.pop_front();
		if(rsp.mSequence == aSeq) {
			arRsp = rsp.mResponse;
			return true;
		}
	}

	return false;
}

}

