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
// Copyright 2013-2019 Automatak, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Automatak LLC license
// this file to you under the Apache License, Version 2.0 (the "License"); you
// may not use this file except in compliance with the License. You may obtain
// a copy of the License at:
// 
//   http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include "opendnp3/gen/StaticAnalogVariation.h"

namespace opendnp3 {

uint8_t StaticAnalogVariationToType(StaticAnalogVariation arg)
{
  return static_cast<uint8_t>(arg);
}
StaticAnalogVariation StaticAnalogVariationFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return StaticAnalogVariation::Group30Var1;
    case(1):
      return StaticAnalogVariation::Group30Var2;
    case(2):
      return StaticAnalogVariation::Group30Var3;
    case(3):
      return StaticAnalogVariation::Group30Var4;
    case(4):
      return StaticAnalogVariation::Group30Var5;
    case(5):
      return StaticAnalogVariation::Group30Var6;
  }
}
char const* StaticAnalogVariationToString(StaticAnalogVariation arg)
{
  switch(arg)
  {
    case(StaticAnalogVariation::Group30Var1):
      return "Group30Var1";
    case(StaticAnalogVariation::Group30Var2):
      return "Group30Var2";
    case(StaticAnalogVariation::Group30Var3):
      return "Group30Var3";
    case(StaticAnalogVariation::Group30Var4):
      return "Group30Var4";
    case(StaticAnalogVariation::Group30Var5):
      return "Group30Var5";
    case(StaticAnalogVariation::Group30Var6):
      return "Group30Var6";
    default:
      return "UNDEFINED";
  }
}

}
