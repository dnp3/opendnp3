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

#include "APDUHexBuilders.h"

#include <opendnp3/app/APDURequest.h>
#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUBuilders.h>

#include "HexConversions.h"

using namespace openpal;
using namespace opendnp3;

namespace hex
{
	std::string ClassTask(FunctionCode fc, uint8_t seq, const ClassField& field)
	{		
		uint8_t buffer[2048];
		APDURequest request(WriteBuffer(buffer, 2048));
		opendnp3::build::ClassRequest(request, fc, field, seq);
		return toHex(request.ToReadOnly());
	}

	std::string IntegrityPoll(uint8_t seq, const ClassField& field)
	{
		return ClassTask(FunctionCode::READ, seq, field);
	}

	std::string ClassPoll(uint8_t seq, PointClass pc)
	{
		return ClassTask(FunctionCode::READ, seq, ClassField(pc));
	}

	std::string ClearRestartIIN(uint8_t seq)
	{
		uint8_t buffer[2048];
		APDURequest request(WriteBuffer(buffer, 2048));
		build::ClearRestartIIN(request, seq);
		return toHex(request.ToReadOnly());
	}

	std::string MeasureDelay(uint8_t seq)
	{
		uint8_t buffer[2048];
		APDURequest request(WriteBuffer(buffer, 2048));
		build::MeasureDelay(request, seq);
		return toHex(request.ToReadOnly());
	}

	std::string EmptyResponse(uint8_t seq, const opendnp3::IINField& iin)
	{
		uint8_t buffer[2048];
		APDUResponse response(WriteBuffer(buffer, 2048));
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, seq));
		response.SetIIN(iin);
		return toHex(response.ToReadOnly());
	}

	std::string NullUnsolicited(uint8_t seq, const IINField& iin)
	{
		uint8_t buffer[2048];
		APDUResponse response(WriteBuffer(buffer, 2048));
		build::NullUnsolicited(response, seq, iin);
		return toHex(response.ToReadOnly());
	}	

	std::string SolicitedConfirm(uint8_t seq)
	{
		return Confirm(seq, false);
	}

	std::string UnsolConfirm(uint8_t seq)
	{
		return Confirm(seq, true);
	}

	std::string Confirm(uint8_t seq, bool unsol)
	{
		uint8_t buffer[2048];
		APDURequest apdu(WriteBuffer(buffer, 2048));
		apdu.SetControl(AppControlField(true, true, false, unsol, seq));
		apdu.SetFunction(FunctionCode::CONFIRM);
		return toHex(apdu.ToReadOnly());
	}
}


