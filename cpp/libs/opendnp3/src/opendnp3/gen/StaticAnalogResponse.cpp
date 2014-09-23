//
//  _   _         ______    _ _ _   _             _ _ _
// | \ | |       |  ____|  | (_) | (_)           | | | |
// |  \| | ___   | |__   __| |_| |_ _ _ __   __ _| | | |
// | . ` |/ _ \  |  __| / _` | | __| | '_ \ / _` | | | |
// | |\  | (_) | | |___| (_| | | |_| | | | | (_| |_|_|_|
// |_| \_|\___/  |______\__,_|_|\__|_|_| |_|\__, (_|_|_)
//                                           __/ |
//                                          |___/
// 
// This file is auto-generated. Do not edit manually
// 
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "StaticAnalogResponse.h"

namespace opendnp3 {

int StaticAnalogResponseToType(StaticAnalogResponse arg)
{
  return static_cast<int>(arg);
}
StaticAnalogResponse StaticAnalogResponseFromType(int arg)
{
  switch(arg)
  {
    case(0):
      return StaticAnalogResponse::Group30Var1;
    case(1):
      return StaticAnalogResponse::Group30Var2;
    case(2):
      return StaticAnalogResponse::Group30Var3;
    case(3):
      return StaticAnalogResponse::Group30Var4;
    case(4):
      return StaticAnalogResponse::Group30Var5;
    case(5):
      return StaticAnalogResponse::Group30Var6;
  }
  return StaticAnalogResponse::Group30Var6;
}

}
