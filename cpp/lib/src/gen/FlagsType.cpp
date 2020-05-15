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

#include "opendnp3/gen/FlagsType.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t FlagsTypeSpec::to_type(FlagsType arg)
{
  return static_cast<uint8_t>(arg);
}

FlagsType FlagsTypeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return FlagsType::DoubleBinaryInput;
    case(0x2):
      return FlagsType::Counter;
    case(0x3):
      return FlagsType::FrozenCounter;
    case(0x4):
      return FlagsType::AnalogInput;
    case(0x5):
      return FlagsType::BinaryOutputStatus;
    case(0x6):
      return FlagsType::AnalogOutputStatus;
    default:
      return FlagsType::BinaryInput;
  }
}

char const* FlagsTypeSpec::to_string(FlagsType arg)
{
  switch(arg)
  {
    case(FlagsType::DoubleBinaryInput):
      return "DoubleBinaryInput";
    case(FlagsType::Counter):
      return "Counter";
    case(FlagsType::FrozenCounter):
      return "FrozenCounter";
    case(FlagsType::AnalogInput):
      return "AnalogInput";
    case(FlagsType::BinaryOutputStatus):
      return "BinaryOutputStatus";
    case(FlagsType::AnalogOutputStatus):
      return "AnalogOutputStatus";
    default:
      return "BinaryInput";
  }
}

char const* FlagsTypeSpec::to_human_string(FlagsType arg)
{
  switch(arg)
  {
    case(FlagsType::DoubleBinaryInput):
      return "DoubleBinaryInput";
    case(FlagsType::Counter):
      return "Counter";
    case(FlagsType::FrozenCounter):
      return "FrozenCounter";
    case(FlagsType::AnalogInput):
      return "AnalogInput";
    case(FlagsType::BinaryOutputStatus):
      return "BinaryOutputStatus";
    case(FlagsType::AnalogOutputStatus):
      return "AnalogOutputStatus";
    default:
      return "BinaryInput";
  }
}

FlagsType FlagsTypeSpec::from_string(const std::string& arg)
{
  if(arg == "DoubleBinaryInput") return FlagsType::DoubleBinaryInput;
  if(arg == "Counter") return FlagsType::Counter;
  if(arg == "FrozenCounter") return FlagsType::FrozenCounter;
  if(arg == "AnalogInput") return FlagsType::AnalogInput;
  if(arg == "BinaryOutputStatus") return FlagsType::BinaryOutputStatus;
  if(arg == "AnalogOutputStatus") return FlagsType::AnalogOutputStatus;
  else return FlagsType::BinaryInput;
}


}
