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

#include "opendnp3/gen/EventCounterVariation.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t EventCounterVariationSpec::to_type(EventCounterVariation arg)
{
  return static_cast<uint8_t>(arg);
}

EventCounterVariation EventCounterVariationSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return EventCounterVariation::Group22Var1;
    case(1):
      return EventCounterVariation::Group22Var2;
    case(2):
      return EventCounterVariation::Group22Var5;
    case(3):
      return EventCounterVariation::Group22Var6;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* EventCounterVariationSpec::to_string(EventCounterVariation arg)
{
  switch(arg)
  {
    case(EventCounterVariation::Group22Var1):
      return "Group22Var1";
    case(EventCounterVariation::Group22Var2):
      return "Group22Var2";
    case(EventCounterVariation::Group22Var5):
      return "Group22Var5";
    case(EventCounterVariation::Group22Var6):
      return "Group22Var6";
    default:
      return "UNDEFINED";
  }
}

char const* EventCounterVariationSpec::to_human_string(EventCounterVariation arg)
{
  switch(arg)
  {
    case(EventCounterVariation::Group22Var1):
      return "Group22Var1";
    case(EventCounterVariation::Group22Var2):
      return "Group22Var2";
    case(EventCounterVariation::Group22Var5):
      return "Group22Var5";
    case(EventCounterVariation::Group22Var6):
      return "Group22Var6";
    default:
      return "UNDEFINED";
  }
}

EventCounterVariation EventCounterVariationSpec::from_string(const std::string& arg)
{
  if(arg == "Group22Var1") return EventCounterVariation::Group22Var1;
  if(arg == "Group22Var2") return EventCounterVariation::Group22Var2;
  if(arg == "Group22Var5") return EventCounterVariation::Group22Var5;
  if(arg == "Group22Var6") return EventCounterVariation::Group22Var6;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
