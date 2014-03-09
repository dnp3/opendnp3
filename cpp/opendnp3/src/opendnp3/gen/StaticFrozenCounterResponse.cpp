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

#include "StaticFrozenCounterResponse.h"

namespace opendnp3
{

std::string StaticFrozenCounterResponseToString(StaticFrozenCounterResponse arg)
{
	switch(arg)
	{
	case(StaticFrozenCounterResponse::Group21Var1):
		return "Group21Var1";
	case(StaticFrozenCounterResponse::Group21Var2):
		return "Group21Var2";
	case(StaticFrozenCounterResponse::Group21Var5):
		return "Group21Var5";
	case(StaticFrozenCounterResponse::Group21Var6):
		return "Group21Var6";
	case(StaticFrozenCounterResponse::Group21Var7):
		return "Group21Var7";
	case(StaticFrozenCounterResponse::Group21Var8):
		return "Group21Var8";
	case(StaticFrozenCounterResponse::Group21Var9):
		return "Group21Var9";
	case(StaticFrozenCounterResponse::Group21Var10):
		return "Group21Var10";
	}
	return "Group21Var10";
}
int StaticFrozenCounterResponseToType(StaticFrozenCounterResponse arg)
{
	return static_cast<int>(arg);
}
StaticFrozenCounterResponse StaticFrozenCounterResponseFromType(int arg)
{
	switch(arg)
	{
	case(0):
		return StaticFrozenCounterResponse::Group21Var1;
	case(1):
		return StaticFrozenCounterResponse::Group21Var2;
	case(2):
		return StaticFrozenCounterResponse::Group21Var5;
	case(3):
		return StaticFrozenCounterResponse::Group21Var6;
	case(4):
		return StaticFrozenCounterResponse::Group21Var7;
	case(5):
		return StaticFrozenCounterResponse::Group21Var8;
	case(6):
		return StaticFrozenCounterResponse::Group21Var9;
	case(7):
		return StaticFrozenCounterResponse::Group21Var10;
	}
	return StaticFrozenCounterResponse::Group21Var10;
}

}
