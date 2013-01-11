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
#ifndef __COMMAND_QUEUE_H_
#define __COMMAND_QUEUE_H_

#include "CommandTypes.h"
#include "INotifier.h"
#include "CommandInterfaces.h"

#include <queue>
#include <mutex>

namespace apl
{
struct CommandData {
	CommandData(apl::CommandTypes aType, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor) :
		mType(aType), mIndex(aIndex), mSequence(aSequence), mpRspAcceptor(apRspAcceptor) {}

	CommandData() {}

	apl::CommandTypes mType;
	size_t mIndex;
	int mSequence;
	IResponseAcceptor* mpRspAcceptor;
};

class CommandQueue : public ICommandAcceptor, public ICommandSource
{
public:
	CommandQueue() : mpNotifier(NULL) {}

	//Implement the ICommandAcceptor interface
	void AcceptCommand(const apl::BinaryOutput& arType, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor);
	void AcceptCommand(const apl::Setpoint& arType, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor);

	void SetNotifier(INotifier* apNotifier);

	size_t Size();

	apl::CommandTypes Next();

	void Read(apl::BinaryOutput& arType, CommandData& arData);
	void Read(apl::Setpoint& arType, CommandData& arData);

	/** Synchronously executes a command, expecting an immediate response from a handler
	*	@return true if there was a command to execute
	*/
	bool ExecuteCommand(ICommandHandler* apHandler);

	/** Immediately respond to a command
	*	@return true if there was a command to respond
	*/
	bool RespondToCommand(CommandStatus aStatus);

protected:
	std::mutex mMutex;
	apl::INotifier* mpNotifier;

	std::queue< apl::BinaryOutput > mBinaryQueue;
	std::queue< apl::Setpoint > mSetpointQueue;

	std::queue< CommandData > mTypeQueue;

	template <typename T>
	void Read(T& arType, CommandData& arData, std::queue<T>& arQueue);

	template <typename T>
	void AcceptCommand(const T& arType, size_t aIndex, std::queue<T>& arQueue, int aSequence, IResponseAcceptor* apRspAcceptor);
};

}
#endif
