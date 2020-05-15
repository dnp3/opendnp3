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

#include "opendnp3/gen/AnalogOutputStatusQuality.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t AnalogOutputStatusQualitySpec::to_type(AnalogOutputStatusQuality arg)
{
  return static_cast<uint8_t>(arg);
}

AnalogOutputStatusQuality AnalogOutputStatusQualitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return AnalogOutputStatusQuality::ONLINE;
    case(0x2):
      return AnalogOutputStatusQuality::RESTART;
    case(0x4):
      return AnalogOutputStatusQuality::COMM_LOST;
    case(0x8):
      return AnalogOutputStatusQuality::REMOTE_FORCED;
    case(0x10):
      return AnalogOutputStatusQuality::LOCAL_FORCED;
    case(0x20):
      return AnalogOutputStatusQuality::OVERRANGE;
    case(0x40):
      return AnalogOutputStatusQuality::REFERENCE_ERR;
    case(0x80):
      return AnalogOutputStatusQuality::RESERVED;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* AnalogOutputStatusQualitySpec::to_string(AnalogOutputStatusQuality arg)
{
  switch(arg)
  {
    case(AnalogOutputStatusQuality::ONLINE):
      return "ONLINE";
    case(AnalogOutputStatusQuality::RESTART):
      return "RESTART";
    case(AnalogOutputStatusQuality::COMM_LOST):
      return "COMM_LOST";
    case(AnalogOutputStatusQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(AnalogOutputStatusQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(AnalogOutputStatusQuality::OVERRANGE):
      return "OVERRANGE";
    case(AnalogOutputStatusQuality::REFERENCE_ERR):
      return "REFERENCE_ERR";
    case(AnalogOutputStatusQuality::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

char const* AnalogOutputStatusQualitySpec::to_human_string(AnalogOutputStatusQuality arg)
{
  switch(arg)
  {
    case(AnalogOutputStatusQuality::ONLINE):
      return "ONLINE";
    case(AnalogOutputStatusQuality::RESTART):
      return "RESTART";
    case(AnalogOutputStatusQuality::COMM_LOST):
      return "COMM_LOST";
    case(AnalogOutputStatusQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(AnalogOutputStatusQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(AnalogOutputStatusQuality::OVERRANGE):
      return "OVERRANGE";
    case(AnalogOutputStatusQuality::REFERENCE_ERR):
      return "REFERENCE_ERR";
    case(AnalogOutputStatusQuality::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

AnalogOutputStatusQuality AnalogOutputStatusQualitySpec::from_string(const std::string& arg)
{
  if(arg == "ONLINE") return AnalogOutputStatusQuality::ONLINE;
  if(arg == "RESTART") return AnalogOutputStatusQuality::RESTART;
  if(arg == "COMM_LOST") return AnalogOutputStatusQuality::COMM_LOST;
  if(arg == "REMOTE_FORCED") return AnalogOutputStatusQuality::REMOTE_FORCED;
  if(arg == "LOCAL_FORCED") return AnalogOutputStatusQuality::LOCAL_FORCED;
  if(arg == "OVERRANGE") return AnalogOutputStatusQuality::OVERRANGE;
  if(arg == "REFERENCE_ERR") return AnalogOutputStatusQuality::REFERENCE_ERR;
  if(arg == "RESERVED") return AnalogOutputStatusQuality::RESERVED;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
