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

#include "opendnp3/gen/EventDoubleBinaryVariation.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t EventDoubleBinaryVariationSpec::to_type(EventDoubleBinaryVariation arg)
{
  return static_cast<uint8_t>(arg);
}

EventDoubleBinaryVariation EventDoubleBinaryVariationSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return EventDoubleBinaryVariation::Group4Var1;
    case(1):
      return EventDoubleBinaryVariation::Group4Var2;
    case(2):
      return EventDoubleBinaryVariation::Group4Var3;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* EventDoubleBinaryVariationSpec::to_string(EventDoubleBinaryVariation arg)
{
  switch(arg)
  {
    case(EventDoubleBinaryVariation::Group4Var1):
      return "Group4Var1";
    case(EventDoubleBinaryVariation::Group4Var2):
      return "Group4Var2";
    case(EventDoubleBinaryVariation::Group4Var3):
      return "Group4Var3";
    default:
      return "UNDEFINED";
  }
}

char const* EventDoubleBinaryVariationSpec::to_human_string(EventDoubleBinaryVariation arg)
{
  switch(arg)
  {
    case(EventDoubleBinaryVariation::Group4Var1):
      return "Group4Var1";
    case(EventDoubleBinaryVariation::Group4Var2):
      return "Group4Var2";
    case(EventDoubleBinaryVariation::Group4Var3):
      return "Group4Var3";
    default:
      return "UNDEFINED";
  }
}

EventDoubleBinaryVariation EventDoubleBinaryVariationSpec::from_string(const std::string& arg)
{
  if(arg == "Group4Var1") return EventDoubleBinaryVariation::Group4Var1;
  if(arg == "Group4Var2") return EventDoubleBinaryVariation::Group4Var2;
  if(arg == "Group4Var3") return EventDoubleBinaryVariation::Group4Var3;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
