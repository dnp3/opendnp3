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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (www.automatak.com) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

#include "opendnp3/gen/EventFrozenCounterVariation.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t EventFrozenCounterVariationSpec::to_type(EventFrozenCounterVariation arg)
{
  return static_cast<uint8_t>(arg);
}

EventFrozenCounterVariation EventFrozenCounterVariationSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return EventFrozenCounterVariation::Group23Var1;
    case(1):
      return EventFrozenCounterVariation::Group23Var2;
    case(2):
      return EventFrozenCounterVariation::Group23Var5;
    case(3):
      return EventFrozenCounterVariation::Group23Var6;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* EventFrozenCounterVariationSpec::to_string(EventFrozenCounterVariation arg)
{
  switch(arg)
  {
    case(EventFrozenCounterVariation::Group23Var1):
      return "Group23Var1";
    case(EventFrozenCounterVariation::Group23Var2):
      return "Group23Var2";
    case(EventFrozenCounterVariation::Group23Var5):
      return "Group23Var5";
    case(EventFrozenCounterVariation::Group23Var6):
      return "Group23Var6";
    default:
      return "UNDEFINED";
  }
}

char const* EventFrozenCounterVariationSpec::to_human_string(EventFrozenCounterVariation arg)
{
  switch(arg)
  {
    case(EventFrozenCounterVariation::Group23Var1):
      return "Group23Var1";
    case(EventFrozenCounterVariation::Group23Var2):
      return "Group23Var2";
    case(EventFrozenCounterVariation::Group23Var5):
      return "Group23Var5";
    case(EventFrozenCounterVariation::Group23Var6):
      return "Group23Var6";
    default:
      return "UNDEFINED";
  }
}

EventFrozenCounterVariation EventFrozenCounterVariationSpec::from_string(const std::string& arg)
{
  if(arg == "Group23Var1") return EventFrozenCounterVariation::Group23Var1;
  if(arg == "Group23Var2") return EventFrozenCounterVariation::Group23Var2;
  if(arg == "Group23Var5") return EventFrozenCounterVariation::Group23Var5;
  if(arg == "Group23Var6") return EventFrozenCounterVariation::Group23Var6;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
