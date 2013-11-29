
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
// you under the terms of the License.
//

#include "AppHeader.h"

#include <assert.h>
#include <sstream>

#include <openpal/ToHex.h>

using namespace std;
using namespace openpal;

#define MACRO_FUNC_CASE(func) case(func): return func;

namespace opendnp3
{

RequestHeader RequestHeader::mInstance;
ResponseHeader ResponseHeader::mInstance;

bool IINField::operator==(const IINField& arRHS) const
{
	return (mLSB == arRHS.mLSB) && (mMSB == arRHS.mMSB);
}

#define MACRO_IIN_TO_STRING(field) if(Get##field()) oss << " "#field;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string IINField::ToString() const
{
	ostringstream oss;
	oss << " IIN: (LSB: " << ByteToHex(GetLSB());

	MACRO_IIN_TO_STRING(AllStations)
	MACRO_IIN_TO_STRING(Class1Events)
	MACRO_IIN_TO_STRING(Class2Events)
	MACRO_IIN_TO_STRING(Class3Events)
	MACRO_IIN_TO_STRING(NeedTime)
	MACRO_IIN_TO_STRING(LocalControl)
	MACRO_IIN_TO_STRING(DeviceTrouble)
	MACRO_IIN_TO_STRING(DeviceRestart)

	oss << ") (MSB: " << ByteToHex(GetMSB());

	MACRO_IIN_TO_STRING(FuncNotSupported)
	MACRO_IIN_TO_STRING(ObjectUnknown)
	MACRO_IIN_TO_STRING(ParameterError)
	MACRO_IIN_TO_STRING(EventBufferOverflow)
	MACRO_IIN_TO_STRING(AlreadyExecuting)
	MACRO_IIN_TO_STRING(ConfigurationCorrupt)
	MACRO_IIN_TO_STRING(Reserved1)
	MACRO_IIN_TO_STRING(Reserved2)
	oss << ")";

	return oss.str();
}
#endif

void IAppHeader::SetControl(uint8_t* apStart, const AppControlField& arControl) const
{
	(*apStart) = 0;
	if(arControl.FIR) (*apStart) |= ACM_FIR;
	if(arControl.FIN) (*apStart) |= ACM_FIN;
	if(arControl.CON) (*apStart) |= ACM_CON;
	if(arControl.UNS) (*apStart) |= ACM_UNS;

	(*apStart) |= (arControl.SEQ % 16);
}

AppControlField IAppHeader::GetControl(const uint8_t* apStart) const
{
	AppControlField f;

	f.FIR = ((*apStart) & ACM_FIR) != 0;
	f.FIN = ((*apStart) & ACM_FIN) != 0;
	f.CON = ((*apStart) & ACM_CON) != 0;
	f.UNS = ((*apStart) & ACM_UNS) != 0;
	f.SEQ = (*apStart) & ACM_SEQ;

	return f;
}



IINField ResponseHeader::GetIIN(const uint8_t* apStart) const
{
	IINField f;

	const uint8_t* pByte = apStart + 2; //start w/ LSB

	f.SetLSB(*pByte);
	f.SetMSB(*(++pByte));

	return f;
}

void ResponseHeader::SetIIN(uint8_t* apStart, const IINField& arIIN) const
{
	uint8_t* pByte = apStart + 2; //start w/ LSB

	*pByte = arIIN.GetLSB();
	*(++pByte) = arIIN.GetMSB();
}

}
