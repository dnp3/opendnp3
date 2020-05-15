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

#include "opendnp3/gen/EventAnalogOutputStatusVariation.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t EventAnalogOutputStatusVariationSpec::to_type(EventAnalogOutputStatusVariation arg)
{
  return static_cast<uint8_t>(arg);
}

EventAnalogOutputStatusVariation EventAnalogOutputStatusVariationSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return EventAnalogOutputStatusVariation::Group42Var1;
    case(1):
      return EventAnalogOutputStatusVariation::Group42Var2;
    case(2):
      return EventAnalogOutputStatusVariation::Group42Var3;
    case(3):
      return EventAnalogOutputStatusVariation::Group42Var4;
    case(4):
      return EventAnalogOutputStatusVariation::Group42Var5;
    case(5):
      return EventAnalogOutputStatusVariation::Group42Var6;
    case(6):
      return EventAnalogOutputStatusVariation::Group42Var7;
    case(7):
      return EventAnalogOutputStatusVariation::Group42Var8;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* EventAnalogOutputStatusVariationSpec::to_string(EventAnalogOutputStatusVariation arg)
{
  switch(arg)
  {
    case(EventAnalogOutputStatusVariation::Group42Var1):
      return "Group42Var1";
    case(EventAnalogOutputStatusVariation::Group42Var2):
      return "Group42Var2";
    case(EventAnalogOutputStatusVariation::Group42Var3):
      return "Group42Var3";
    case(EventAnalogOutputStatusVariation::Group42Var4):
      return "Group42Var4";
    case(EventAnalogOutputStatusVariation::Group42Var5):
      return "Group42Var5";
    case(EventAnalogOutputStatusVariation::Group42Var6):
      return "Group42Var6";
    case(EventAnalogOutputStatusVariation::Group42Var7):
      return "Group42Var7";
    case(EventAnalogOutputStatusVariation::Group42Var8):
      return "Group42Var8";
    default:
      return "UNDEFINED";
  }
}

char const* EventAnalogOutputStatusVariationSpec::to_human_string(EventAnalogOutputStatusVariation arg)
{
  switch(arg)
  {
    case(EventAnalogOutputStatusVariation::Group42Var1):
      return "Group42Var1";
    case(EventAnalogOutputStatusVariation::Group42Var2):
      return "Group42Var2";
    case(EventAnalogOutputStatusVariation::Group42Var3):
      return "Group42Var3";
    case(EventAnalogOutputStatusVariation::Group42Var4):
      return "Group42Var4";
    case(EventAnalogOutputStatusVariation::Group42Var5):
      return "Group42Var5";
    case(EventAnalogOutputStatusVariation::Group42Var6):
      return "Group42Var6";
    case(EventAnalogOutputStatusVariation::Group42Var7):
      return "Group42Var7";
    case(EventAnalogOutputStatusVariation::Group42Var8):
      return "Group42Var8";
    default:
      return "UNDEFINED";
  }
}

EventAnalogOutputStatusVariation EventAnalogOutputStatusVariationSpec::from_string(const std::string& arg)
{
  if(arg == "Group42Var1") return EventAnalogOutputStatusVariation::Group42Var1;
  if(arg == "Group42Var2") return EventAnalogOutputStatusVariation::Group42Var2;
  if(arg == "Group42Var3") return EventAnalogOutputStatusVariation::Group42Var3;
  if(arg == "Group42Var4") return EventAnalogOutputStatusVariation::Group42Var4;
  if(arg == "Group42Var5") return EventAnalogOutputStatusVariation::Group42Var5;
  if(arg == "Group42Var6") return EventAnalogOutputStatusVariation::Group42Var6;
  if(arg == "Group42Var7") return EventAnalogOutputStatusVariation::Group42Var7;
  if(arg == "Group42Var8") return EventAnalogOutputStatusVariation::Group42Var8;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
