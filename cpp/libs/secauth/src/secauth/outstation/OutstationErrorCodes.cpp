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

#include "OutstationErrorCodes.h"

namespace secauth
{
	OutstationErrorCategory OutstationErrorCategory::instance;

	std::string OutstationErrorCategory::message(int ev) const
	{
		switch (ev)
		{
		case(static_cast<int>(OutstationError::UNEXPECTED_CHALLENGE_REPLY)) :
			return "Unexpected challenge response in IDLE state";
		case(static_cast<int>(OutstationError::MALFORMED_AUTH_REQUEST)) :
			return "Malformed auth request";
		case(static_cast<int>(OutstationError::MAX_ERROR_MESSAGES_EXCEEDED)) :
			return "Max auth error messages exceeded";
		default:
			return "unknown outstation error";
		}
	}

	std::error_code make_error_code(OutstationError err)
	{
		return std::error_code(static_cast<int>(err), OutstationErrorCategory::Instance());
	}

}

