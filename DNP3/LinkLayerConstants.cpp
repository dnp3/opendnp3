//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one
// or more contributor license agreements. See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  Green Enery Corp licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
#include "LinkLayerConstants.h"


namespace apl
{
namespace dnp
{

#define MACRO_ENUM_CASE(name) case(name): return name;

FuncCodes ByteToFuncCode(uint8_t aCode)
{
	switch(aCode) {
		MACRO_ENUM_CASE(FC_PRI_RESET_LINK_STATES)
		MACRO_ENUM_CASE(FC_PRI_TEST_LINK_STATES)
		MACRO_ENUM_CASE(FC_PRI_CONFIRMED_USER_DATA)
		MACRO_ENUM_CASE(FC_PRI_UNCONFIRMED_USER_DATA)
		MACRO_ENUM_CASE(FC_PRI_REQUEST_LINK_STATUS)
		MACRO_ENUM_CASE(FC_SEC_ACK)
		MACRO_ENUM_CASE(FC_SEC_NACK)
		MACRO_ENUM_CASE(FC_SEC_LINK_STATUS)
		MACRO_ENUM_CASE(FC_SEC_NOT_SUPPORTED)

		// Used as an error code for return values
	default: return FC_INVALID;
	}
}

#define MACRO_ENUM_STRING_CASE(name) case(name): return #name;

std::string FuncCodeToString(FuncCodes aCode)
{
	switch(aCode) {
		MACRO_ENUM_STRING_CASE(FC_PRI_RESET_LINK_STATES)
		MACRO_ENUM_STRING_CASE(FC_PRI_TEST_LINK_STATES)
		MACRO_ENUM_STRING_CASE(FC_PRI_CONFIRMED_USER_DATA)
		MACRO_ENUM_STRING_CASE(FC_PRI_UNCONFIRMED_USER_DATA)
		MACRO_ENUM_STRING_CASE(FC_PRI_REQUEST_LINK_STATUS)
		MACRO_ENUM_STRING_CASE(FC_SEC_ACK)
		MACRO_ENUM_STRING_CASE(FC_SEC_NACK)
		MACRO_ENUM_STRING_CASE(FC_SEC_LINK_STATUS)
		MACRO_ENUM_STRING_CASE(FC_SEC_NOT_SUPPORTED)
	default: return "Unknown function";
	}
}

}
}
