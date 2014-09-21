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

#include "EventFrozenCounterResponse.h"

namespace opendnp3 {

int EventFrozenCounterResponseToType(EventFrozenCounterResponse arg)
{
  return static_cast<int>(arg);
}
EventFrozenCounterResponse EventFrozenCounterResponseFromType(int arg)
{
  switch(arg)
  {
    case(0):
      return EventFrozenCounterResponse::Group23Var1;
    case(1):
      return EventFrozenCounterResponse::Group23Var2;
    case(2):
      return EventFrozenCounterResponse::Group23Var5;
    case(3):
      return EventFrozenCounterResponse::Group23Var6;
  }
  return EventFrozenCounterResponse::Group23Var6;
}

}
