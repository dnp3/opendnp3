
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

#include "CommandProcessorAdapter.h"
#include "Conversions.h"
#include "Lock.h"




using namespace System::Threading;

namespace DNP3
{
namespace Adapter
{

void ResponseRouter::Set(gcroot < Future<CommandStatus> ^ > * apFuture, opendnp3::CommandResponse cr)
{
	std::auto_ptr < gcroot < Future<CommandStatus> ^ >> ptr(apFuture);
	(*apFuture)->Set(Conversions::convertCommandStatus(cr.GetStatus()));
}

CommandProcessorAdapter::CommandProcessorAdapter(opendnp3::ICommandProcessor* apProxy) : mpProxy(apProxy)
{

}

IFuture<CommandStatus> ^ CommandProcessorAdapter::SelectAndOperate(ControlRelayOutputBlock ^ command, System::UInt32 index)
{
	return this->SelectAndOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::SelectAndOperate(AnalogOutputInt32 ^ command, System::UInt32 index)
{
	return this->SelectAndOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::SelectAndOperate(AnalogOutputInt16 ^ command, System::UInt32 index)
{
	return this->SelectAndOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::SelectAndOperate(AnalogOutputFloat32 ^ command, System::UInt32 index)
{
	return this->SelectAndOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::SelectAndOperate(AnalogOutputDouble64 ^ command, System::UInt32 index)
{
	return this->SelectAndOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::DirectOperate(ControlRelayOutputBlock ^ command, System::UInt32 index)
{
	return this->DirectOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::DirectOperate(AnalogOutputInt32 ^ command, System::UInt32 index)
{
	return this->DirectOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::DirectOperate(AnalogOutputInt16 ^ command, System::UInt32 index)
{
	return this->DirectOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::DirectOperate(AnalogOutputFloat32 ^ command, System::UInt32 index)
{
	return this->DirectOperateT(command, index);
}

IFuture<CommandStatus> ^ CommandProcessorAdapter::DirectOperate(AnalogOutputDouble64 ^ command, System::UInt32 index)
{
	return this->DirectOperateT(command, index);
}





}
}


