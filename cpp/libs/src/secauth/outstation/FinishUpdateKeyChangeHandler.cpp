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

#include "FinishUpdateKeyChangeHandler.h"

using namespace openpal;
using namespace opendnp3;

namespace secauth
{

bool FinishUpdateKeyChangeHandler::IsAllowed(uint32_t headerCount, GroupVariation gv, QualifierCode qc)
{
	if (qc != QualifierCode::UINT16_FREE_FORMAT) return false; // all data is free format

	switch (gv)
	{
		case(GroupVariation::Group120Var13) : // this must be the first object header
			return headerCount == 0;
		case(GroupVariation::Group120Var14) : // either of these is allowed in position 1
		case(GroupVariation::Group120Var15) :
			return headerCount == 1;			
		default:
			return false;
	}
}	

IINField FinishUpdateKeyChangeHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var13& value, const openpal::RSlice& object)
{
	this->keyChange = value;
	return IINField();
}

IINField FinishUpdateKeyChangeHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var14& value, const openpal::RSlice& object)
{
	this->authData = value.digitalSignature;
	this->m_result = Result::DIGITAL_SIGNATURE;
	return IINField();
}

IINField FinishUpdateKeyChangeHandler::ProcessHeader(const FreeFormatHeader& header, const Group120Var15& value, const openpal::RSlice& object)
{
	this->authData = value.hmacValue;
	this->m_result = Result::HMAC;
	return IINField();
}

}



