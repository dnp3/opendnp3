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

#include <openpal/StaticBuffer.h>
#include <opendnp3/app/APDURequest.h>
#include <opendnp3/app/APDUResponse.h>
#include <opendnp3/app/APDUBuilders.h>

#include "HexConversions.h"

using namespace openpal;
using namespace opendnp3;

namespace hex
{

	std::string IntegrityPoll(uint8_t seq, int mask)
	{
		StaticBuffer<100> buffer;
		APDURequest request(buffer.GetWriteBuffer());
		opendnp3::build::ReadIntegrity(request, mask, seq);
		return toHex(request.ToReadOnly());
	}

	std::string EmptyResponse(uint8_t seq)
	{
		StaticBuffer<4> buffer;
		APDUResponse response(buffer.GetWriteBuffer());
		response.SetFunction(FunctionCode::RESPONSE);
		response.SetControl(AppControlField(true, true, false, false, seq));
		return toHex(response.ToReadOnly());
	}
}


