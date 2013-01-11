
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
#include "CommandAcceptorAdapter.h"
#include "Conversions.h"
#include "Lock.h"

#include <APL/CommandInterfaces.h>


using namespace System::Threading;

namespace DNP3
{	
namespace Adapter
{

ResponseRouter::ResponseRouter() : mMutex(gcnew System::Object()), mSequence(0)
{

}

void ResponseRouter::AcceptResponse(const apl::CommandResponse& arRsp, int aSequence)
{
	Lock lock(mMutex);
	std::map<int, gcroot< Future<CommandStatus>^ > >::iterator i = mMap.find(aSequence);
	if(i != mMap.end()) {
		CommandStatus status = Conversions::convertCommandStatus(arRsp.mResult);
		i->second->Set(status);
	}
}

int ResponseRouter::RegisterFuture(Future<CommandStatus>^ future)
{
	Lock lock(mMutex);
	int seq = this->mSequence++;
	this->mMap[seq] = gcroot< Future<CommandStatus>^ >(future);
	return seq;
}

CommandAcceptorAdapter::CommandAcceptorAdapter(apl::ICommandAcceptor* apProxy) : mpProxy(apProxy), mpRouter(new ResponseRouter())
{

}

CommandAcceptorAdapter::~CommandAcceptorAdapter()
{
	delete mpRouter;
}

IFuture<CommandStatus>^ CommandAcceptorAdapter::AcceptCommand(BinaryOutput^ command, System::UInt32 index)
{
	Future<CommandStatus>^  future = gcnew Future<CommandStatus>();

	int seq = mpRouter->RegisterFuture(future);
	apl::BinaryOutput cmd = Conversions::convertBO(command);
	
	mpProxy->AcceptCommand(cmd, index, seq, mpRouter);

	return future;
}

IFuture<CommandStatus>^ CommandAcceptorAdapter::AcceptCommand(Setpoint^ command, System::UInt32 index)
{
	Future<CommandStatus>^  future = gcnew Future<CommandStatus>();

	int seq = mpRouter->RegisterFuture(future);
	apl::Setpoint cmd = Conversions::convertSP(command);
	
	mpProxy->AcceptCommand(cmd, index, seq, mpRouter);

	return future;
}



}}


