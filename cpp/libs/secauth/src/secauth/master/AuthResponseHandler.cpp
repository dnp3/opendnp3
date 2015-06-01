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

#include "AuthResponseHandler.h"

using namespace opendnp3;

namespace secauth
{
	

AuthResponseHandler::AuthResponseHandler(
	const openpal::ReadBufferView& apdu_,
	const APDUResponseHeader& header,	
	IAuthResponseReceiver& authRespReceiver
	) :
	apdu(apdu_),
	apduheader(header),	
	pAuthRespReceiver(&authRespReceiver)
{}
		

IINField AuthResponseHandler::ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var1& value)
{
	pAuthRespReceiver->OnAuthChallenge(apdu, apduheader, value);
	return IINField::Empty();
}

IINField AuthResponseHandler::ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var7& value)
{
	pAuthRespReceiver->OnAuthError(apdu, apduheader, value);
	return IINField::Empty();
}

bool AuthResponseHandler::IsAllowed(uint32_t count, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc)
{
	// only 1 header allowed regardless of type
	if (count != 0)
	{
		return false;
	}

	// certain types allowed with certain qualifiers
	switch (gv)
	{	
		case(GroupVariation::Group120Var1) :
		case(GroupVariation::Group120Var6) :
			return qc == QualifierCode::UINT16_FREE_FORMAT;
		default:
			return false;
	}
}


}



