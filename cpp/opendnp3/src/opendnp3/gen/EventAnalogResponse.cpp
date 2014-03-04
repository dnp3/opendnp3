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

#include "EventAnalogResponse.h"

namespace opendnp3 {

std::string EventAnalogResponseToString(EventAnalogResponse arg)
{
  switch(arg)
  {
    case(EventAnalogResponse::Group32Var1):
      return "Group32Var1";
    case(EventAnalogResponse::Group32Var2):
      return "Group32Var2";
    case(EventAnalogResponse::Group32Var3):
      return "Group32Var3";
    case(EventAnalogResponse::Group32Var4):
      return "Group32Var4";
    case(EventAnalogResponse::Group32Var5):
      return "Group32Var5";
    case(EventAnalogResponse::Group32Var6):
      return "Group32Var6";
    case(EventAnalogResponse::Group32Var7):
      return "Group32Var7";
    case(EventAnalogResponse::Group32Var8):
      return "Group32Var8";
  }
  return "Group32Var8";
}
int EventAnalogResponseToType(EventAnalogResponse arg)
{
  return static_cast<int>(arg);
}
EventAnalogResponse EventAnalogResponseFromType(int arg)
{
  switch(arg)
  {
    case(0):
      return EventAnalogResponse::Group32Var1;
    case(1):
      return EventAnalogResponse::Group32Var2;
    case(2):
      return EventAnalogResponse::Group32Var3;
    case(3):
      return EventAnalogResponse::Group32Var4;
    case(4):
      return EventAnalogResponse::Group32Var5;
    case(5):
      return EventAnalogResponse::Group32Var6;
    case(6):
      return EventAnalogResponse::Group32Var7;
    case(7):
      return EventAnalogResponse::Group32Var8;
  }
  return EventAnalogResponse::Group32Var8;
}

}
