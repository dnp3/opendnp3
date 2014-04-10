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
#include "opendnp3/outstation/CommandActionAdapter.h"
#include "opendnp3/outstation/CommandResponseHandler.h"

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
		context(executor, root, lower, commandHandler, database, buffers)
{
	context.pDatabase->SetEventBuffer(context.eventBuffer);
}
	
void NewOutstation::OnLowerLayerUp()
{
	if (!context.isOnline)
	{
		context.SetOnline();
	}
}
	
void NewOutstation::OnLowerLayerDown()
{
	if (context.isOnline)
	{
		context.SetOffline();
	}
}

void NewOutstation::OnReceive(const openpal::ReadOnlyBuffer& buffer)
{
	if (context.isOnline && !context.isSending)
	{
		APDURecord request;
		auto result = APDUHeaderParser::ParseRequest(buffer, request);
		if (result == APDUHeaderParser::Result::OK)
		{						
			APDUResponse response(context.txBuffer.GetWriteBuffer());
			response.SetFunction(FunctionCode::RESPONSE);			
			IINField iin = BuildResponse(request, response);			
			response.SetControl(request.control.ToByte());
			response.SetIIN(iin);
			context.isSending = true;

			//context.pLower->BeginTransmit(response.ToReadOnly());			
			auto output = response.ToReadOnly();						
			auto lambda = [this, output]() { this->context.pLower->BeginTransmit(output); };
			context.pExecutor->PostLambda(lambda);
		}
	}
}
	
void NewOutstation::OnSendResult(bool isSucccess)
{
	if (context.isOnline && context.isSending)
	{
		context.isSending = false;
	}
}

IINField NewOutstation::BuildResponse(const APDURecord& request, APDUResponse& response)
{
	switch (request.function)
	{
		case(FunctionCode::READ):
			return HandleRead(request, response);
		case(FunctionCode::DIRECT_OPERATE) :
			return HandleDirectOperate(request, response);
		default:
			return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}	
}

IINField NewOutstation::HandleRead(const APDURecord& request, APDUResponse& response)
{
	context.rspContext.Reset();
	ReadHandler handler(context.logger, context.rspContext);
	auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger, APDUParser::Context(false)); // don't expect range/count context on a READ
	if (result == APDUParser::Result::OK)
	{
		// Do a transaction on the database (lock) for multi-threaded environments
		// if the request contained static variations, we double buffer (copy) the entire static database.
		// this ensures that an multi-fragmented responses see a consistent snapshot
		openpal::Transaction tx(context.pDatabase);
		context.pDatabase->DoubleBuffer();
		context.rspContext.Load(response);
		return handler.Errors();
	}
	else
	{
		context.rspContext.Reset();
		return IINField(IINBit::PARAM_ERROR);
	}
}

IINField NewOutstation::HandleDirectOperate(const APDURecord& request, APDUResponse& response)
{
	if (request.objects.Size() > response.Remaining())
	{
		FORMAT_LOG_BLOCK(context.logger, flags::WARN, "Igonring command request due to payload size of %i", request.objects.Size());
		return IINField(IINBit::PARAM_ERROR);
	}
	else
	{
		CommandActionAdapter adapter(context.pCommandHandler, false);
		CommandResponseHandler handler(context.logger, 1, &adapter, response);	// TODO support multiple controls
		auto result = APDUParser::ParseTwoPass(request.objects, &handler, &context.logger);
		return IINFromParseResult(result);
	}
}
	
}


