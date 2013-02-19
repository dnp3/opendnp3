
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
#include "SlaveCommandHandlerAdapter.h"
#include "Conversions.h"



namespace DNP3
{	
namespace Adapter
{

SlaveCommandHandlerAdapter::SlaveCommandHandlerAdapter(DNP3::Interface::ICommandHandler^ proxy) : proxy(proxy)
{

}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Select(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Select(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::Operate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->Operate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::ControlRelayOutputBlock& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputInt32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputInt16& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputFloat32& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}

opendnp3::CommandStatus SlaveCommandHandlerAdapter::DirectOperate(const opendnp3::AnalogOutputDouble64& arCommand, size_t aIndex)
{
	return Conversions::convertCommandStatus(proxy->DirectOperate(Conversions::convertCommand(arCommand), aIndex));
}




	
}}

