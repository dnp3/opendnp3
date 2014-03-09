//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// Copyright 2013 Automatak LLC
//
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
//
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "StaticAnalogOutputStatusResponse.h"

namespace opendnp3
{

std::string StaticAnalogOutputStatusResponseToString(StaticAnalogOutputStatusResponse arg)
{
	switch(arg)
	{
	case(StaticAnalogOutputStatusResponse::Group40Var1):
		return "Group40Var1";
	case(StaticAnalogOutputStatusResponse::Group40Var2):
		return "Group40Var2";
	case(StaticAnalogOutputStatusResponse::Group40Var3):
		return "Group40Var3";
	case(StaticAnalogOutputStatusResponse::Group40Var4):
		return "Group40Var4";
	}
	return "Group40Var4";
}
int StaticAnalogOutputStatusResponseToType(StaticAnalogOutputStatusResponse arg)
{
	return static_cast<int>(arg);
}
StaticAnalogOutputStatusResponse StaticAnalogOutputStatusResponseFromType(int arg)
{
	switch(arg)
	{
	case(0):
		return StaticAnalogOutputStatusResponse::Group40Var1;
	case(1):
		return StaticAnalogOutputStatusResponse::Group40Var2;
	case(2):
		return StaticAnalogOutputStatusResponse::Group40Var3;
	case(3):
		return StaticAnalogOutputStatusResponse::Group40Var4;
	}
	return StaticAnalogOutputStatusResponse::Group40Var4;
}

}
