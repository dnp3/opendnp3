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
// Copyright 2013-2020 Automatak, LLC
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

#include "opendnp3/gen/StaticAnalogOutputStatusVariation.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t StaticAnalogOutputStatusVariationSpec::to_type(StaticAnalogOutputStatusVariation arg)
{
  return static_cast<uint8_t>(arg);
}

StaticAnalogOutputStatusVariation StaticAnalogOutputStatusVariationSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return StaticAnalogOutputStatusVariation::Group40Var1;
    case(1):
      return StaticAnalogOutputStatusVariation::Group40Var2;
    case(2):
      return StaticAnalogOutputStatusVariation::Group40Var3;
    case(3):
      return StaticAnalogOutputStatusVariation::Group40Var4;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* StaticAnalogOutputStatusVariationSpec::to_string(StaticAnalogOutputStatusVariation arg)
{
  switch(arg)
  {
    case(StaticAnalogOutputStatusVariation::Group40Var1):
      return "Group40Var1";
    case(StaticAnalogOutputStatusVariation::Group40Var2):
      return "Group40Var2";
    case(StaticAnalogOutputStatusVariation::Group40Var3):
      return "Group40Var3";
    case(StaticAnalogOutputStatusVariation::Group40Var4):
      return "Group40Var4";
    default:
      return "UNDEFINED";
  }
}

char const* StaticAnalogOutputStatusVariationSpec::to_human_string(StaticAnalogOutputStatusVariation arg)
{
  switch(arg)
  {
    case(StaticAnalogOutputStatusVariation::Group40Var1):
      return "Group40Var1";
    case(StaticAnalogOutputStatusVariation::Group40Var2):
      return "Group40Var2";
    case(StaticAnalogOutputStatusVariation::Group40Var3):
      return "Group40Var3";
    case(StaticAnalogOutputStatusVariation::Group40Var4):
      return "Group40Var4";
    default:
      return "UNDEFINED";
  }
}

StaticAnalogOutputStatusVariation StaticAnalogOutputStatusVariationSpec::from_string(const std::string& arg)
{
  if(arg == "Group40Var1") return StaticAnalogOutputStatusVariation::Group40Var1;
  if(arg == "Group40Var2") return StaticAnalogOutputStatusVariation::Group40Var2;
  if(arg == "Group40Var3") return StaticAnalogOutputStatusVariation::Group40Var3;
  if(arg == "Group40Var4") return StaticAnalogOutputStatusVariation::Group40Var4;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
