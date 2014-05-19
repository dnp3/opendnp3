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

#include "APDUBuilders.h"

#include "opendnp3/app/PointClass.h"
#include "opendnp3/objects/Group60.h"
#include "opendnp3/app/IINField.h"

#include <openpal/Serialization.h>


namespace opendnp3
{
namespace build
{

void ReadIntegrity(APDURequest& request, int classMask, uint8_t seq)
{
	ClassRequest(request, FunctionCode::READ, classMask, seq);
}

void ClassRequest(APDURequest& request, FunctionCode fc, int classMask, uint8_t seq)
{
	request.SetControl(AppControlField(true, true, false, false, seq));
	request.SetFunction(fc);
	WriteClassHeaders(request, classMask);
}

void WriteClassHeaders(APDURequest& request, int classMask)
{
	auto writer = request.GetWriter();
	if (classMask & CLASS_1)
	{
		writer.WriteHeader(Group60Var2::ID, QualifierCode::ALL_OBJECTS);
	}
	if (classMask & CLASS_2)
	{
		writer.WriteHeader(Group60Var3::ID, QualifierCode::ALL_OBJECTS);
	}
	if (classMask & CLASS_3)
	{
		writer.WriteHeader(Group60Var4::ID, QualifierCode::ALL_OBJECTS);
	}
	if (classMask & CLASS_0)
	{
		writer.WriteHeader(Group60Var1::ID, QualifierCode::ALL_OBJECTS);
	}
}

void DisableUnsolicited(APDURequest& request, uint8_t seq)
{
	ClassRequest(request, FunctionCode::DISABLE_UNSOLICITED, ALL_EVENT_CLASSES, seq);	
}

void EnableUnsolicited(APDURequest& request, int classMask, uint8_t seq)
{
	ClassRequest(request, FunctionCode::ENABLE_UNSOLICITED, ALL_EVENT_CLASSES & classMask, seq);
}

void ClearRestartIIN(APDURequest& request, uint8_t seq)
{
	request.SetFunction(FunctionCode::WRITE);
	request.SetControl(AppControlField(true, true, false, false, seq));
	auto writer = request.GetWriter();
	auto iter = writer.IterateOverSingleBitfield<openpal::UInt8>(GroupVariationID(80, 1), QualifierCode::UINT8_START_STOP, static_cast<uint8_t>(IINBit::DEVICE_RESTART));
	iter.Write(true);	
	iter.Complete();
}

}
}

