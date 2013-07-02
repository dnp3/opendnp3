
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

#include <opendnp3/CommandStatus.h>

namespace opendnp3
{

#define MACRO_CASE_DECLARE(type) case(type): return type;

CommandStatus IntToCommandStatus(int aField)
{
	switch(aField) {
		MACRO_CASE_DECLARE(CS_SUCCESS)
		MACRO_CASE_DECLARE(CS_TIMEOUT)
		MACRO_CASE_DECLARE(CS_NO_SELECT)
		MACRO_CASE_DECLARE(CS_FORMAT_ERROR)
		MACRO_CASE_DECLARE(CS_NOT_SUPPORTED)
		MACRO_CASE_DECLARE(CS_ALREADY_ACTIVE)
		MACRO_CASE_DECLARE(CS_HARDWARE_ERROR)
		MACRO_CASE_DECLARE(CS_LOCAL)
		MACRO_CASE_DECLARE(CS_TOO_MANY_OPS)
		MACRO_CASE_DECLARE(CS_NOT_AUTHORIZED)

	default:
		return CS_UNDEFINED;
	}

}

#define TO_STRING_CASE(c) case (c): return #c;

#ifndef OPENDNP3_STRIP_LOG_MESSAGES
std::string ToString(CommandStatus aType)
{
	switch(aType) {
		TO_STRING_CASE(CS_SUCCESS)
		TO_STRING_CASE(CS_TIMEOUT)
		TO_STRING_CASE(CS_NO_SELECT)
		TO_STRING_CASE(CS_FORMAT_ERROR)
		TO_STRING_CASE(CS_NOT_SUPPORTED)
		TO_STRING_CASE(CS_ALREADY_ACTIVE)
		TO_STRING_CASE(CS_HARDWARE_ERROR)
		TO_STRING_CASE(CS_LOCAL)
		TO_STRING_CASE(CS_TOO_MANY_OPS)
		TO_STRING_CASE(CS_NOT_AUTHORIZED)
	default:
		return "Unknown";
	}
}
#endif

}

