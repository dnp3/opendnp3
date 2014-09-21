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

#include "EventAnalogOutputStatusResponse.h"

namespace opendnp3 {

int EventAnalogOutputStatusResponseToType(EventAnalogOutputStatusResponse arg)
{
  return static_cast<int>(arg);
}
EventAnalogOutputStatusResponse EventAnalogOutputStatusResponseFromType(int arg)
{
  switch(arg)
  {
    case(0):
      return EventAnalogOutputStatusResponse::Group42Var1;
    case(1):
      return EventAnalogOutputStatusResponse::Group42Var2;
    case(2):
      return EventAnalogOutputStatusResponse::Group42Var3;
    case(3):
      return EventAnalogOutputStatusResponse::Group42Var4;
    case(4):
      return EventAnalogOutputStatusResponse::Group42Var5;
    case(5):
      return EventAnalogOutputStatusResponse::Group42Var6;
    case(6):
      return EventAnalogOutputStatusResponse::Group42Var7;
    case(7):
      return EventAnalogOutputStatusResponse::Group42Var8;
  }
  return EventAnalogOutputStatusResponse::Group42Var8;
}

}
