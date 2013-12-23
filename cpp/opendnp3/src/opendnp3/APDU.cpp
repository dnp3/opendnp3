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

#include "APDU.h"

#include <assert.h>

#include "AppControlField.h"

namespace opendnp3
{

APDU::APDU() : buffer()
{

}

APDU::APDU(const openpal::WriteBuffer& aBuffer) : buffer(aBuffer)
{
	assert(aBuffer.Size() >= 2); // need a control & function at a minimum
}

void APDU::SetFunction(FunctionCode code)
{
	assert(buffer.IsNotEmpty());
	buffer[1] = FunctionCodeToType(code);
}

FunctionCode APDU::GetFunction() const
{
	assert(buffer.IsNotEmpty());
	return FunctionCodeFromType(buffer[1]);
}

AppControlField APDU::GetControl() const
{
	assert(buffer.IsNotEmpty());
	return AppControlField(buffer[0]);
}

void APDU::SetControl(const AppControlField& control)
{
	buffer[0] = control.ToByte();
}

openpal::ReadOnlyBuffer APDU::ToReadOnly() const
{
	return buffer.ToReadOnly();
}

}

