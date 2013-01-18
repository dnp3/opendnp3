
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
#include "CommandProcessorAdapter.h"
#include "Conversions.h"
#include "Lock.h"




using namespace System::Threading;

namespace DNP3
{	
namespace Adapter
{

void ResponseRouter::Set(gcroot<Future<CommandStatus>^> future, apl::CommandResponse cr)
{	
	future->Set(Conversions::convertCommandStatus(cr.mResult));
}

CommandProcessorAdapter::CommandProcessorAdapter(apl::dnp::ICommandProcessor* apProxy) : mpProxy(apProxy)
{

}

IFuture<CommandStatus>^ CommandProcessorAdapter::Select(BinaryOutput^ command, System::UInt32 index)
{
	auto future = gcnew Future<CommandStatus>();
	
	apl::BinaryOutput cmd = Conversions::convertBO(command);

	auto wrapper = gcroot<Future<CommandStatus>^>(future);
	
	mpProxy->Select(cmd, index, std::bind(&ResponseRouter::Set, wrapper, std::placeholders::_1)); 

	return future;
}

IFuture<CommandStatus>^ CommandProcessorAdapter::Select(Setpoint^ command, System::UInt32 index)
{
	auto future = gcnew Future<CommandStatus>();
	
	apl::Setpoint cmd = Conversions::convertSP(command);

	auto wrapper = gcroot<Future<CommandStatus>^>(future);
	
	mpProxy->Select(cmd, index, std::bind(&ResponseRouter::Set, wrapper, std::placeholders::_1)); 

	return future;
}

IFuture<CommandStatus>^ CommandProcessorAdapter::Operate(BinaryOutput^ command, System::UInt32 index)
{
	auto future = gcnew Future<CommandStatus>();
	
	apl::BinaryOutput cmd = Conversions::convertBO(command);

	auto wrapper = gcroot<Future<CommandStatus>^>(future);
	
	mpProxy->Operate(cmd, index, std::bind(&ResponseRouter::Set, wrapper, std::placeholders::_1)); 

	return future;
}

IFuture<CommandStatus>^ CommandProcessorAdapter::Operate(Setpoint^ command, System::UInt32 index)
{
	auto future = gcnew Future<CommandStatus>();
	
	apl::Setpoint cmd = Conversions::convertSP(command);

	auto wrapper = gcroot<Future<CommandStatus>^>(future);
	
	mpProxy->Operate(cmd, index, std::bind(&ResponseRouter::Set, wrapper, std::placeholders::_1)); 

	return future;
}





}}


