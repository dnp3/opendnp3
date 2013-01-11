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

#include "SlaveCommandAcceptorAdapter.h"
#include "Conversions.h"

namespace DNP3
{	
namespace Adapter
{

SlaveCommandAcceptorAdapterWrapper::SlaveCommandAcceptorAdapterWrapper(ICommandAcceptor^ proxy) : 
	pAdapter(new SlaveCommandAcceptorAdapter(proxy))
{

}

SlaveCommandAcceptorAdapterWrapper::~SlaveCommandAcceptorAdapterWrapper()
{ 
	delete pAdapter;
}
	
SlaveCommandAcceptorAdapter::SlaveCommandAcceptorAdapter(DNP3::Interface::ICommandAcceptor^ proxy) : proxy(proxy)
{

}

void SlaveCommandAcceptorAdapter::AcceptCommand(const apl::BinaryOutput& arCmd, size_t aIndex, int aSequence, apl::IResponseAcceptor* apRspAcceptor)
{
	IFuture<CommandStatus>^ future = proxy->AcceptCommand(Conversions::convertBO(arCmd), aIndex);

	ResponseDelegateAdapter^ rda = gcnew ResponseDelegateAdapter(apRspAcceptor, aSequence);

	FutureCallback<CommandStatus>^ callback = gcnew FutureCallback<CommandStatus>(rda, &ResponseDelegateAdapter::OnResult);

	future->Listen(callback);
}

void SlaveCommandAcceptorAdapter::AcceptCommand(const apl::Setpoint& arCmd, size_t aIndex, int aSequence, apl::IResponseAcceptor* apRspAcceptor)
{
	IFuture<CommandStatus>^ future = proxy->AcceptCommand(Conversions::convertSP(arCmd), aIndex);	

	ResponseDelegateAdapter^ rda = gcnew ResponseDelegateAdapter(apRspAcceptor, aSequence);

	FutureCallback<CommandStatus>^ callback = gcnew FutureCallback<CommandStatus>(rda, &ResponseDelegateAdapter::OnResult);

	future->Listen(callback);
}

ResponseDelegateAdapter::ResponseDelegateAdapter(apl::IResponseAcceptor* apRspAcceptor, int aSequence) : 
	mpRspAcceptor(apRspAcceptor), 
	mSequence(aSequence)	
{}

void ResponseDelegateAdapter::OnResult(CommandStatus status)
{
	apl::CommandResponse cr(Conversions::convertCommandStatus(status));
	mpRspAcceptor->AcceptResponse(cr, mSequence);
}
	
}}

