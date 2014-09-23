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

#include "PointClass.h"

namespace opendnp3 {

uint8_t PointClassToType(PointClass arg)
{
  return static_cast<uint8_t>(arg);
}
PointClass PointClassFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return PointClass::Class0;
    case(0x2):
      return PointClass::Class1;
    case(0x4):
      return PointClass::Class2;
    case(0x8):
      return PointClass::Class3;
  }
  return PointClass::Class3;
}
char const* PointClassToString(PointClass arg)
{
  switch(arg)
  {
    case(PointClass::Class0):
      return "Class0";
    case(PointClass::Class1):
      return "Class1";
    case(PointClass::Class2):
      return "Class2";
    case(PointClass::Class3):
      return "Class3";
  }
  return "Class3";
}

}
