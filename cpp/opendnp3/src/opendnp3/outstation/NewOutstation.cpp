/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include "NewOutstation.h"

#include "opendnp3/app/APDUHeaderParser.h"
#include "opendnp3/app/APDUResponse.h"
#include "opendnp3/app/APDUParser.h"

#include "opendnp3/outstation/ReadHandler.h"
#include "opendnp3/outstation/IINHelpers.h"

using namespace openpal;

namespace opendnp3
{

NewOutstation::NewOutstation(
		IExecutor& executor,
		openpal::LogRoot& root,
		openpal::ILowerLayer& lower,
		ICommandHandler& commandHandler,
		Database& database,
		EventBufferFacade& buffers) :
	isOnline(false),
	isSending(false),
	logger(root.GetLogger()),
	pExecutor(&executor),
	pLower(&lower),
	pDatabase(&database),
	eventBuffer(buffers),
	rspContext(&database, &eventBuffer, StaticResponseTypes())
{
	pDatabase->SetEventBuffer(eventBuffer);
}
	
void NewOutstation::OnLowerLayerUp()
{
	if (!isOnline)
	{
		isOnline = true;
	}
}
	
void NewOutstation::OnLowerLayerDown()
{
	if (isOnline)
	{
		isOnline = false;
		isSending = false;
	}
}

void NewOutstation::OnReceive(const openpal::ReadOnlyBuffer& buffer)
{
	if (isOnline && !isSending)
	{
		APDURecord request;
		auto result = APDUHeaderParser::ParseRequest(buffer, request);
		if (result == APDUHeaderParser::Result::OK)
		{						
			APDUResponse response(txBuffer.GetWriteBuffer());			
			response.SetFunction(FunctionCode::RESPONSE);			
			IINField iin = BuildResponse(request, response);			
			response.SetControl(request.control.ToByte());
			response.SetIIN(iin);
			isSending = true;
			pLower->BeginTransmit(response.ToReadOnly());
			/* Make this configurable?
			auto output = response.ToReadOnly();						
			auto lambda = [this, output]() { this->pLower->BeginTransmit(output); };
			pExecutor->PostLambda(lambda);
			*/
		}
	}
}
	
void NewOutstation::OnSendResult(bool isSucccess)
{
	if (isOnline && isSending)
	{
		isSending = false;
	}
}

IINField NewOutstation::BuildResponse(const APDURecord& request, APDUResponse& response)
{
	switch (request.function)
	{
		case(FunctionCode::READ):
			return HandleRead(request, response);
		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField NewOutstation::HandleRead(const APDURecord& request, APDUResponse& response)
{
	rspContext.Reset();
	ReadHandler handler(logger, rspContext);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &logger, APDUParser::Context(false)); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{
		// Do a transaction on the database (lock) for multi-threaded environments
		// if the request contained static variations, we double buffer (copy) the entire static database.
		// this ensures that an multi-fragmented responses see a consistent snapshot
		openpal::Transaction tx(pDatabase);
		pDatabase->DoubleBuffer();
		rspContext.Load(response);
		return handler.Errors();
	}
	else
	{
		rspContext.Reset();
		return IINField(IINBit::PARAM_ERROR);
	}
}
	
}


