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

#ifndef OPENDNP3_EVENTANALOGOUTPUTSTATUSVARIATION_H
#define OPENDNP3_EVENTANALOGOUTPUTSTATUSVARIATION_H

#include <cstdint>
#include <string>

namespace opendnp3 {

enum class EventAnalogOutputStatusVariation : uint8_t
{
  Group42Var1 = 0,
  Group42Var2 = 1,
  Group42Var3 = 2,
  Group42Var4 = 3,
  Group42Var5 = 4,
  Group42Var6 = 5,
  Group42Var7 = 6,
  Group42Var8 = 7
};

struct EventAnalogOutputStatusVariationSpec
{
  using enum_type_t = EventAnalogOutputStatusVariation;

  static uint8_t to_type(EventAnalogOutputStatusVariation arg);
  static EventAnalogOutputStatusVariation from_type(uint8_t arg);
  static char const* to_string(EventAnalogOutputStatusVariation arg);
  static char const* to_human_string(EventAnalogOutputStatusVariation arg);
  static EventAnalogOutputStatusVariation from_string(const std::string& arg);
};

}

#endif
