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

#include "secauth/outstation/Permissions.h"

using namespace opendnp3;

namespace secauth
{

Permissions::Permissions(bitfield_t mask) : permissions(mask)
{}

Permissions Permissions::AllowNothing()
{
	return Permissions();
}

Permissions Permissions::AllowAll()
{
	return Permissions(~static_cast<bitfield_t>(0));
}

void Permissions::Allow(FunctionCode code)
{
	this->permissions |= GetMask(code);
}

bool Permissions::IsAllowed(opendnp3::FunctionCode code) const
{
	return (GetMask(code) & permissions) != 0;
}

Permissions::bitfield_t Permissions::GetMask(opendnp3::FunctionCode code)
{
	switch (code)
	{
	case(FunctionCode::CONFIRM) :
		return Bit(0);
	case(FunctionCode::READ):
		return Bit(1);
	case(FunctionCode::WRITE) :
		return Bit(2);
	case(FunctionCode::SELECT) :
		return Bit(3);
	case(FunctionCode::OPERATE) :
		return Bit(4);
	case(FunctionCode::DIRECT_OPERATE) :
		return Bit(5);
	case(FunctionCode::DIRECT_OPERATE_NR) :
		return Bit(6);
	case(FunctionCode::IMMED_FREEZE) :
		return Bit(7);
	case(FunctionCode::IMMED_FREEZE_NR) :
		return Bit(8);
	case(FunctionCode::FREEZE_CLEAR) :
		return Bit(9);
	case(FunctionCode::FREEZE_CLEAR_NR) :
		return Bit(10);
	case(FunctionCode::FREEZE_AT_TIME) :
		return Bit(11);
	case(FunctionCode::FREEZE_AT_TIME_NR) :
		return Bit(12);
	case(FunctionCode::COLD_RESTART) :
		return Bit(13);
	case(FunctionCode::WARM_RESTART) :
		return Bit(14);
	case(FunctionCode::INITIALIZE_DATA) :
		return Bit(15);
	case(FunctionCode::INITIALIZE_APPLICATION) :
		return Bit(16);
	case(FunctionCode::START_APPLICATION) :
		return Bit(17);
	case(FunctionCode::STOP_APPLICATION) :
		return Bit(18);
	case(FunctionCode::SAVE_CONFIGURATION) :
		return Bit(19);
	case(FunctionCode::ENABLE_UNSOLICITED) :
		return Bit(20);
	case(FunctionCode::DISABLE_UNSOLICITED) :
		return Bit(21);
	case(FunctionCode::ASSIGN_CLASS) :
		return Bit(22);
	case(FunctionCode::DELAY_MEASURE) :
		return Bit(23);
	case(FunctionCode::RECORD_CURRENT_TIME) :
		return Bit(24);
	case(FunctionCode::OPEN_FILE) :
		return Bit(25);
	case(FunctionCode::CLOSE_FILE) :
		return Bit(26);
	case(FunctionCode::DELETE_FILE) :
		return Bit(27);
	case(FunctionCode::GET_FILE_INFO) :
		return Bit(28);
	case(FunctionCode::AUTHENTICATE_FILE) :
		return Bit(29);
	case(FunctionCode::ABORT_FILE) :
		return Bit(30);
	default:
		return 0;
	}
}

}



