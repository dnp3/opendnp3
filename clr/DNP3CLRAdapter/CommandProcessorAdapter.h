
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
// you under the terms of the License.
//

#ifndef __COMMAND_PROCESSOR_ADAPTER_H_
#define __COMMAND_PROCESSOR_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <map>

#include <opendnp3/ICommandProcessor.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private class ResponseRouter
{
public:
	static void Set(gcroot < Future<CommandStatus> ^ > * apFuture, opendnp3::CommandResponse cr);
};

private ref class CommandProcessorAdapter : public ICommandProcessor
{
public:

	CommandProcessorAdapter(opendnp3::ICommandProcessor* apProxy);

	virtual IFuture<CommandStatus> ^ SelectAndOperate(ControlRelayOutputBlock ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ SelectAndOperate(AnalogOutputInt32 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ SelectAndOperate(AnalogOutputInt16 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ SelectAndOperate(AnalogOutputFloat32 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ SelectAndOperate(AnalogOutputDouble64 ^ command, System::UInt32 index);

	virtual IFuture<CommandStatus> ^ DirectOperate(ControlRelayOutputBlock ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ DirectOperate(AnalogOutputInt32 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ DirectOperate(AnalogOutputInt16 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ DirectOperate(AnalogOutputFloat32 ^ command, System::UInt32 index);
	virtual IFuture<CommandStatus> ^ DirectOperate(AnalogOutputDouble64 ^ command, System::UInt32 index);

private:

	template <class T>
	IFuture<CommandStatus> ^ SelectAndOperateT(T ^ command, System::UInt32 index) {
		auto future = gcnew Future<CommandStatus>();
		auto cmd = Conversions::convertCommand(command);
		auto pWrapper = new gcroot < Future<CommandStatus> ^ > (future);
		mpProxy->SelectAndOperate(cmd, index, std::bind(&ResponseRouter::Set, pWrapper, std::placeholders::_1));
		return future;
	}

	template <class T>
	IFuture<CommandStatus> ^ DirectOperateT(T ^ command, System::UInt32 index) {
		auto future = gcnew Future<CommandStatus>();
		auto cmd = Conversions::convertCommand(command);
		auto pWrapper = new gcroot < Future<CommandStatus> ^ > (future);
		mpProxy->DirectOperate(cmd, index, std::bind(&ResponseRouter::Set, pWrapper, std::placeholders::_1));
		return future;
	}

	opendnp3::ICommandProcessor* mpProxy;
};

}
}

#endif
