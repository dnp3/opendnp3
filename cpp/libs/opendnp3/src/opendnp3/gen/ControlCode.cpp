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

#include "ControlCode.h"

namespace opendnp3 {

uint8_t ControlCodeToType(ControlCode arg)
{
  return static_cast<uint8_t>(arg);
}
ControlCode ControlCodeFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return ControlCode::NUL;
    case(0x1):
      return ControlCode::PULSE;
    case(0x3):
      return ControlCode::LATCH_ON;
    case(0x4):
      return ControlCode::LATCH_OFF;
    case(0x41):
      return ControlCode::PULSE_CLOSE;
    case(0x81):
      return ControlCode::PULSE_TRIP;
    case(0xFF):
      return ControlCode::UNDEFINED;
  }
  return ControlCode::UNDEFINED;
}
char const* ControlCodeToString(ControlCode arg)
{
  switch(arg)
  {
    case(ControlCode::NUL):
      return "NUL";
    case(ControlCode::PULSE):
      return "PULSE";
    case(ControlCode::LATCH_ON):
      return "LATCH_ON";
    case(ControlCode::LATCH_OFF):
      return "LATCH_OFF";
    case(ControlCode::PULSE_CLOSE):
      return "PULSE_CLOSE";
    case(ControlCode::PULSE_TRIP):
      return "PULSE_TRIP";
    case(ControlCode::UNDEFINED):
      return "UNDEFINED";
  }
  return "UNDEFINED";
}

}
