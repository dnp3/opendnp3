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

#include "AuthErrorHandler.h"

using namespace opendnp3;

namespace secauth
{
	AuthErrorHandler::AuthErrorHandler() : hasError(false)
	{
		
	}

	bool AuthErrorHandler::GetError(opendnp3::Group120Var7& errorOut) const
	{
		if (hasError)
		{
			errorOut = this->error;
		}

		return hasError;
	}

	opendnp3::IINField AuthErrorHandler::ProcessHeader(const opendnp3::FreeFormatHeader& header, const opendnp3::Group120Var7& value)
	{
		hasError = true;
		this->error = value;
		return IINField::Empty();
	}

	bool AuthErrorHandler::IsAllowed(uint32_t headerCount, opendnp3::GroupVariation gv, opendnp3::QualifierCode qc)
	{
		return (headerCount == 0) && (gv == GroupVariation::Group120Var7) && (qc == QualifierCode::UINT16_FREE_FORMAT);
	}


}



