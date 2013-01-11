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
#ifndef __COMMAND_INTERFACES_H_
#define __COMMAND_INTERFACES_H_

#include "CommandTypes.h"
#include "INotifier.h"

namespace apl
{

/** Interface that defines how reponses to command requests are reported to client code. Master client codes
*   provides an instance of this class when calling ICommandAcceptor::AcceptCommand.
*/
class IResponseAcceptor
{
public:
	virtual ~IResponseAcceptor() {};

	/**
	* This function is called when the command request succeeds or fails. 
	* @param arResponse Contains a description of whether the command succeeded/failed.
	* @param aSequence ID that is echoed for correlation purposes
	*
	*/
	virtual void AcceptResponse(const CommandResponse& arResponse, int aSequence) = 0;
};

/** Defines the interface an object must implement to handle requests. Typically this interface is used by a client of a DNP3 master to issue control
	requests and by a slave to handle them as they are received from a master.
*/
class ICommandAcceptor
{
public:
	virtual ~ICommandAcceptor() {};

	/** Asynchronous request that a BinaryOutput be executed by the stack. The stack will call back with the supplied IResponseAcceptor.
	*
	*	@param arCommand The BinaryOuput (CROB) to execute
	*   @param aIndex The index of the command
	*   @param aSequence An ID used to correlate the request with the response. User supplied.
	*   @param apRspAcceptor The interface used to make the callback
	*
	*/
	virtual void AcceptCommand(const BinaryOutput& arCommand, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor) = 0;

	/** Asynchronous request that a Setpoint be executed by the stack. The stack will call back with the supplied IResponseAcceptor.
	*
	*	@param arCommand The Setpoint (AnalogOutput in dnp3) to execute
	*   @param aIndex The index of the command
	*   @param aSequence An ID used to correlate the request with the response. User supplied.
	*   @param apRspAcceptor The interface used to make the callback
	*
	*/
	virtual void AcceptCommand(const Setpoint& arCommand, size_t aIndex, int aSequence, IResponseAcceptor* apRspAcceptor) = 0;
};


/**
	Defines an interface that an application can implement to handle incoming controls. If the application
	needs to handle controls that may last for extended periods of time it will need to use the more
	complicated ICommandAcceptor/IResponseAcceptor interfaces.
 */
class ICommandHandler
{
public:
	virtual ~ICommandHandler() {}
	virtual CommandStatus HandleControl(BinaryOutput& aControl, size_t aIndex) = 0;
	virtual CommandStatus HandleControl(Setpoint& aControl, size_t aIndex) = 0;
};

class FixedCommandHandler : public ICommandHandler
{
public:
	FixedCommandHandler(CommandStatus aStatus) : mStatus(aStatus) {}

	CommandStatus HandleControl(BinaryOutput&, size_t) {
		return mStatus;
	}
	CommandStatus HandleControl(Setpoint&, size_t) {
		return mStatus;
	}

	CommandStatus mStatus;
};


/**
	Interface to the "command source" of the communications stack. When a command is recieved by the stack it
	will call Notify() on the passed in notifier. When Notify() is called (or on a schedule) the application
	should call bool moreCommands = ExecuteCommand(&handler); until moreCommands is false, which means that all
	of the commands available have been handled.

 */
class ICommandSource
{
public:
	virtual ~ICommandSource() {}
	virtual void SetNotifier(INotifier* apNotifier) = 0;

	//	Grab one command off the queue and call the ICommandHandler for the command.
	// This is a blocking call.
	//
	virtual bool ExecuteCommand(ICommandHandler* apHandler) = 0;
};

}

#endif
