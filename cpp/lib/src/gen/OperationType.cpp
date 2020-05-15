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

#include "opendnp3/gen/OperationType.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t OperationTypeSpec::to_type(OperationType arg)
{
  return static_cast<uint8_t>(arg);
}

OperationType OperationTypeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return OperationType::NUL;
    case(0x1):
      return OperationType::PULSE_ON;
    case(0x2):
      return OperationType::PULSE_OFF;
    case(0x3):
      return OperationType::LATCH_ON;
    case(0x4):
      return OperationType::LATCH_OFF;
    default:
      return OperationType::Undefined;
  }
}

char const* OperationTypeSpec::to_string(OperationType arg)
{
  switch(arg)
  {
    case(OperationType::NUL):
      return "NUL";
    case(OperationType::PULSE_ON):
      return "PULSE_ON";
    case(OperationType::PULSE_OFF):
      return "PULSE_OFF";
    case(OperationType::LATCH_ON):
      return "LATCH_ON";
    case(OperationType::LATCH_OFF):
      return "LATCH_OFF";
    default:
      return "Undefined";
  }
}

char const* OperationTypeSpec::to_human_string(OperationType arg)
{
  switch(arg)
  {
    case(OperationType::NUL):
      return "NUL";
    case(OperationType::PULSE_ON):
      return "PULSE_ON";
    case(OperationType::PULSE_OFF):
      return "PULSE_OFF";
    case(OperationType::LATCH_ON):
      return "LATCH_ON";
    case(OperationType::LATCH_OFF):
      return "LATCH_OFF";
    default:
      return "Undefined";
  }
}

OperationType OperationTypeSpec::from_string(const std::string& arg)
{
  if(arg == "NUL") return OperationType::NUL;
  if(arg == "PULSE_ON") return OperationType::PULSE_ON;
  if(arg == "PULSE_OFF") return OperationType::PULSE_OFF;
  if(arg == "LATCH_ON") return OperationType::LATCH_ON;
  if(arg == "LATCH_OFF") return OperationType::LATCH_OFF;
  else return OperationType::Undefined;
}


}
