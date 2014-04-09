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

using namespace openpal;

namespace opendnp3
{

NewOutstation::NewOutstation(IExecutor& executor, openpal::ILowerLayer& lower, Database& database, EventBufferFacade& buffers) :
	isOnline(false),
	isSending(false),
	pExecutor(&executor),
	pLower(&lower),
	eventBuffer(buffers),
	rspContext(&database, &eventBuffer, StaticResponseTypes())
{}
	
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
			response.SetControl(request.control);
			response.SetFunction(FunctionCode::RESPONSE);
			auto writer = response.GetWriter();
			IINField iin = BuildResponse(request, writer);
			response.SetIIN(iin);
			isSending = true;
			pLower->BeginTransmit(response.ToReadOnly());
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

IINField NewOutstation::BuildResponse(const APDURecord& request, ObjectWriter& writer)
{
	if (request.function == FunctionCode::READ)
	{
		return IINField::Empty;
	}
	else
	{
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}
}
	
}


