
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
#include "CommandQueue.h"

#include <assert.h>

namespace apl
{


apl::CommandTypes CommandQueue::Next()
{
	std::unique_lock<std::mutex> lock(mMutex);
	if(mTypeQueue.size() > 0) {
		return mTypeQueue.front().mType;
	}
	else return CT_NONE;
}

template < typename T >
void CommandQueue::Read(T& arType, CommandData& arData, std::queue<T>& arQueue)
{
	std::unique_lock<std::mutex> lock(mMutex);
	assert(mTypeQueue.front().mType == arType.GetType());
	assert(arQueue.size() > 0);
	arType = arQueue.front();
	arData = mTypeQueue.front();
	arQueue.pop();
	mTypeQueue.pop();
}

template <typename T>
void CommandQueue::AcceptCommand(const T& arType, size_t aIndex, std::queue<T>& arQueue, int aSequence, IResponseAcceptor* apRspAcceptor)
{
	{
		std::unique_lock<std::mutex> lock(mMutex);
		arQueue.push(arType);
		mTypeQueue.push(CommandData(arType.GetType(), aIndex, aSequence, apRspAcceptor));
	}
	if(mpNotifier != NULL) mpNotifier->Notify();
}

bool CommandQueue::RespondToCommand(CommandStatus aStatus)
{
	FixedCommandHandler handler(aStatus);
	return this->ExecuteCommand(&handler);
}

bool CommandQueue::ExecuteCommand(ICommandHandler* apHandler)
{

	CommandData info;
	CommandResponse rsp;
	rsp.mResult = apl::CS_NOT_SUPPORTED;

	switch(this->Next()) {
	case(apl::CT_BINARY_OUTPUT): {
			apl::BinaryOutput bo;
			Read(bo, info);
			rsp.mResult = apHandler->HandleControl(bo, info.mIndex);
			break;
		}

	case(apl::CT_SETPOINT): {
			apl::Setpoint sp;
			Read(sp, info);
			rsp.mResult = apHandler->HandleControl(sp, info.mIndex);
			break;
		}
	default:
		return false;

	}
	info.mpRspAcceptor->AcceptResponse(rsp, info.mSequence);
	return true;
}

void CommandQueue::AcceptCommand(const apl::BinaryOutput& arType, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor)
{
	AcceptCommand<apl::BinaryOutput>(arType, aIndex, mBinaryQueue, aSequence, apRspAcceptor);
}
void CommandQueue::AcceptCommand(const apl::Setpoint& arType, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor)
{
	AcceptCommand<apl::Setpoint>(arType, aIndex, mSetpointQueue, aSequence, apRspAcceptor);
}

void CommandQueue::SetNotifier(INotifier* apNotifier)
{
	assert(mpNotifier == NULL);
	assert(apNotifier != NULL);
	mpNotifier = apNotifier;
}

size_t CommandQueue::Size()
{
	std::unique_lock<std::mutex> lock(mMutex);
	return mTypeQueue.size();
}

void CommandQueue::Read(apl::BinaryOutput& arType, CommandData& arData)
{
	return Read<apl::BinaryOutput>(arType, arData, mBinaryQueue);
}
void CommandQueue::Read(apl::Setpoint& arType, CommandData& arData)
{
	return Read<apl::Setpoint>(arType, arData, mSetpointQueue);
}

}
