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

#include "opendnp3/gen/DoubleBitBinaryQuality.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t DoubleBitBinaryQualitySpec::to_type(DoubleBitBinaryQuality arg)
{
  return static_cast<uint8_t>(arg);
}

DoubleBitBinaryQuality DoubleBitBinaryQualitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return DoubleBitBinaryQuality::ONLINE;
    case(0x2):
      return DoubleBitBinaryQuality::RESTART;
    case(0x4):
      return DoubleBitBinaryQuality::COMM_LOST;
    case(0x8):
      return DoubleBitBinaryQuality::REMOTE_FORCED;
    case(0x10):
      return DoubleBitBinaryQuality::LOCAL_FORCED;
    case(0x20):
      return DoubleBitBinaryQuality::CHATTER_FILTER;
    case(0x40):
      return DoubleBitBinaryQuality::STATE1;
    case(0x80):
      return DoubleBitBinaryQuality::STATE2;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* DoubleBitBinaryQualitySpec::to_string(DoubleBitBinaryQuality arg)
{
  switch(arg)
  {
    case(DoubleBitBinaryQuality::ONLINE):
      return "ONLINE";
    case(DoubleBitBinaryQuality::RESTART):
      return "RESTART";
    case(DoubleBitBinaryQuality::COMM_LOST):
      return "COMM_LOST";
    case(DoubleBitBinaryQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(DoubleBitBinaryQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(DoubleBitBinaryQuality::CHATTER_FILTER):
      return "CHATTER_FILTER";
    case(DoubleBitBinaryQuality::STATE1):
      return "STATE1";
    case(DoubleBitBinaryQuality::STATE2):
      return "STATE2";
    default:
      return "UNDEFINED";
  }
}

char const* DoubleBitBinaryQualitySpec::to_human_string(DoubleBitBinaryQuality arg)
{
  switch(arg)
  {
    case(DoubleBitBinaryQuality::ONLINE):
      return "ONLINE";
    case(DoubleBitBinaryQuality::RESTART):
      return "RESTART";
    case(DoubleBitBinaryQuality::COMM_LOST):
      return "COMM_LOST";
    case(DoubleBitBinaryQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(DoubleBitBinaryQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(DoubleBitBinaryQuality::CHATTER_FILTER):
      return "CHATTER_FILTER";
    case(DoubleBitBinaryQuality::STATE1):
      return "STATE1";
    case(DoubleBitBinaryQuality::STATE2):
      return "STATE2";
    default:
      return "UNDEFINED";
  }
}

DoubleBitBinaryQuality DoubleBitBinaryQualitySpec::from_string(const std::string& arg)
{
  if(arg == "ONLINE") return DoubleBitBinaryQuality::ONLINE;
  if(arg == "RESTART") return DoubleBitBinaryQuality::RESTART;
  if(arg == "COMM_LOST") return DoubleBitBinaryQuality::COMM_LOST;
  if(arg == "REMOTE_FORCED") return DoubleBitBinaryQuality::REMOTE_FORCED;
  if(arg == "LOCAL_FORCED") return DoubleBitBinaryQuality::LOCAL_FORCED;
  if(arg == "CHATTER_FILTER") return DoubleBitBinaryQuality::CHATTER_FILTER;
  if(arg == "STATE1") return DoubleBitBinaryQuality::STATE1;
  if(arg == "STATE2") return DoubleBitBinaryQuality::STATE2;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
