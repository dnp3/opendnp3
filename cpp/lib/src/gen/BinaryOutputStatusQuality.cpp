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

#include "opendnp3/gen/BinaryOutputStatusQuality.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t BinaryOutputStatusQualitySpec::to_type(BinaryOutputStatusQuality arg)
{
  return static_cast<uint8_t>(arg);
}

BinaryOutputStatusQuality BinaryOutputStatusQualitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return BinaryOutputStatusQuality::ONLINE;
    case(0x2):
      return BinaryOutputStatusQuality::RESTART;
    case(0x4):
      return BinaryOutputStatusQuality::COMM_LOST;
    case(0x8):
      return BinaryOutputStatusQuality::REMOTE_FORCED;
    case(0x10):
      return BinaryOutputStatusQuality::LOCAL_FORCED;
    case(0x20):
      return BinaryOutputStatusQuality::RESERVED1;
    case(0x40):
      return BinaryOutputStatusQuality::RESERVED2;
    case(0x80):
      return BinaryOutputStatusQuality::STATE;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* BinaryOutputStatusQualitySpec::to_string(BinaryOutputStatusQuality arg)
{
  switch(arg)
  {
    case(BinaryOutputStatusQuality::ONLINE):
      return "ONLINE";
    case(BinaryOutputStatusQuality::RESTART):
      return "RESTART";
    case(BinaryOutputStatusQuality::COMM_LOST):
      return "COMM_LOST";
    case(BinaryOutputStatusQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(BinaryOutputStatusQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(BinaryOutputStatusQuality::RESERVED1):
      return "RESERVED1";
    case(BinaryOutputStatusQuality::RESERVED2):
      return "RESERVED2";
    case(BinaryOutputStatusQuality::STATE):
      return "STATE";
    default:
      return "UNDEFINED";
  }
}

char const* BinaryOutputStatusQualitySpec::to_human_string(BinaryOutputStatusQuality arg)
{
  switch(arg)
  {
    case(BinaryOutputStatusQuality::ONLINE):
      return "ONLINE";
    case(BinaryOutputStatusQuality::RESTART):
      return "RESTART";
    case(BinaryOutputStatusQuality::COMM_LOST):
      return "COMM_LOST";
    case(BinaryOutputStatusQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(BinaryOutputStatusQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(BinaryOutputStatusQuality::RESERVED1):
      return "RESERVED1";
    case(BinaryOutputStatusQuality::RESERVED2):
      return "RESERVED2";
    case(BinaryOutputStatusQuality::STATE):
      return "STATE";
    default:
      return "UNDEFINED";
  }
}

BinaryOutputStatusQuality BinaryOutputStatusQualitySpec::from_string(const std::string& arg)
{
  if(arg == "ONLINE") return BinaryOutputStatusQuality::ONLINE;
  if(arg == "RESTART") return BinaryOutputStatusQuality::RESTART;
  if(arg == "COMM_LOST") return BinaryOutputStatusQuality::COMM_LOST;
  if(arg == "REMOTE_FORCED") return BinaryOutputStatusQuality::REMOTE_FORCED;
  if(arg == "LOCAL_FORCED") return BinaryOutputStatusQuality::LOCAL_FORCED;
  if(arg == "RESERVED1") return BinaryOutputStatusQuality::RESERVED1;
  if(arg == "RESERVED2") return BinaryOutputStatusQuality::RESERVED2;
  if(arg == "STATE") return BinaryOutputStatusQuality::STATE;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
