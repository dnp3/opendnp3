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

#include "Permissions.h"

using namespace opendnp3;

namespace secauth
{
	
Permissions::Permissions(bool allowByDefault) : permissions(allowByDefault ? ALL : NOTHING)
{}

	
Permissions Permissions::AllowNothing()
{
	return Permissions(false);
}

Permissions Permissions::AllowAll()
{
	return Permissions(true);
}
	
void Permissions::Allow(FunctionCode code)
{
	auto mask = GetMask(code);
	if (mask.VALID) 
	{
		this->permissions |= mask.VALUE;		
	}
}

void Permissions::Deny(FunctionCode code)
{
	auto mask = GetMask(code);
	if (mask.VALID)
	{
		this->permissions &= ~mask.VALUE;
	}
}

bool Permissions::IsAllowed(opendnp3::FunctionCode code) const
{
	auto mask = GetMask(code);
	if (!mask.VALID)
	{
		return false;		
	}
		
	return (this->permissions & mask.VALUE) != 0;	
}

Permissions::Mask Permissions::GetMask(opendnp3::FunctionCode code)
{	
	switch (code)
	{		
		case(FunctionCode::CONFIRM) :
			return Mask::Bit(0);						
		case(FunctionCode::READ):
			return Mask::Bit(1);
		case(FunctionCode::WRITE) :
			return Mask::Bit(2);
		case(FunctionCode::SELECT) :
			return Mask::Bit(3);
		case(FunctionCode::OPERATE) :
			return Mask::Bit(4);
		case(FunctionCode::DIRECT_OPERATE) :
			return Mask::Bit(5);
		case(FunctionCode::DIRECT_OPERATE_NR) :
			return Mask::Bit(6);
		case(FunctionCode::IMMED_FREEZE) :
			return Mask::Bit(7);
		case(FunctionCode::IMMED_FREEZE_NR) :
			return Mask::Bit(8);
		case(FunctionCode::FREEZE_CLEAR) :
			return Mask::Bit(9);
		case(FunctionCode::FREEZE_CLEAR_NR) :
			return Mask::Bit(10);
		case(FunctionCode::FREEZE_AT_TIME) :
			return Mask::Bit(11);
		case(FunctionCode::FREEZE_AT_TIME_NR) :
			return Mask::Bit(12);
		case(FunctionCode::COLD_RESTART) :
			return Mask::Bit(13);
		case(FunctionCode::WARM_RESTART) :
			return Mask::Bit(14);
		case(FunctionCode::INITIALIZE_DATA) :
			return Mask::Bit(15);
		case(FunctionCode::INITIALIZE_APPLICATION) :
			return Mask::Bit(16);
		case(FunctionCode::START_APPLICATION) :
			return Mask::Bit(17);
		case(FunctionCode::STOP_APPLICATION) :
			return Mask::Bit(18);
		case(FunctionCode::SAVE_CONFIGURATION) :
			return Mask::Bit(19);
		case(FunctionCode::ENABLE_UNSOLICITED) :
			return Mask::Bit(20);
		case(FunctionCode::DISABLE_UNSOLICITED) :
			return Mask::Bit(21);
		case(FunctionCode::ASSIGN_CLASS) :
			return Mask::Bit(22);
		case(FunctionCode::DELAY_MEASURE) :
			return Mask::Bit(23);
		case(FunctionCode::RECORD_CURRENT_TIME) :
			return Mask::Bit(24);
		case(FunctionCode::OPEN_FILE) :
			return Mask::Bit(25);
		case(FunctionCode::CLOSE_FILE) :
			return Mask::Bit(26);
		case(FunctionCode::DELETE_FILE) :
			return Mask::Bit(27);
		case(FunctionCode::GET_FILE_INFO) :
			return Mask::Bit(28);
		case(FunctionCode::AUTHENTICATE_FILE) :
			return Mask::Bit(29);
		case(FunctionCode::ABORT_FILE) :
			return Mask::Bit(30);
		default:
			return Mask::None();
	}
}

}



