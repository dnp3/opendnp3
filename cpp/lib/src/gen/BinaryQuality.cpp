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

#include "opendnp3/gen/BinaryQuality.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t BinaryQualitySpec::to_type(BinaryQuality arg)
{
  return static_cast<uint8_t>(arg);
}

BinaryQuality BinaryQualitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return BinaryQuality::ONLINE;
    case(0x2):
      return BinaryQuality::RESTART;
    case(0x4):
      return BinaryQuality::COMM_LOST;
    case(0x8):
      return BinaryQuality::REMOTE_FORCED;
    case(0x10):
      return BinaryQuality::LOCAL_FORCED;
    case(0x20):
      return BinaryQuality::CHATTER_FILTER;
    case(0x40):
      return BinaryQuality::RESERVED;
    case(0x80):
      return BinaryQuality::STATE;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* BinaryQualitySpec::to_string(BinaryQuality arg)
{
  switch(arg)
  {
    case(BinaryQuality::ONLINE):
      return "ONLINE";
    case(BinaryQuality::RESTART):
      return "RESTART";
    case(BinaryQuality::COMM_LOST):
      return "COMM_LOST";
    case(BinaryQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(BinaryQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(BinaryQuality::CHATTER_FILTER):
      return "CHATTER_FILTER";
    case(BinaryQuality::RESERVED):
      return "RESERVED";
    case(BinaryQuality::STATE):
      return "STATE";
    default:
      return "UNDEFINED";
  }
}

char const* BinaryQualitySpec::to_human_string(BinaryQuality arg)
{
  switch(arg)
  {
    case(BinaryQuality::ONLINE):
      return "ONLINE";
    case(BinaryQuality::RESTART):
      return "RESTART";
    case(BinaryQuality::COMM_LOST):
      return "COMM_LOST";
    case(BinaryQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(BinaryQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(BinaryQuality::CHATTER_FILTER):
      return "CHATTER_FILTER";
    case(BinaryQuality::RESERVED):
      return "RESERVED";
    case(BinaryQuality::STATE):
      return "STATE";
    default:
      return "UNDEFINED";
  }
}

BinaryQuality BinaryQualitySpec::from_string(const std::string& arg)
{
  if(arg == "ONLINE") return BinaryQuality::ONLINE;
  if(arg == "RESTART") return BinaryQuality::RESTART;
  if(arg == "COMM_LOST") return BinaryQuality::COMM_LOST;
  if(arg == "REMOTE_FORCED") return BinaryQuality::REMOTE_FORCED;
  if(arg == "LOCAL_FORCED") return BinaryQuality::LOCAL_FORCED;
  if(arg == "CHATTER_FILTER") return BinaryQuality::CHATTER_FILTER;
  if(arg == "RESERVED") return BinaryQuality::RESERVED;
  if(arg == "STATE") return BinaryQuality::STATE;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
