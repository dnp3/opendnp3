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

#include "opendnp3/gen/CounterQuality.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t CounterQualitySpec::to_type(CounterQuality arg)
{
  return static_cast<uint8_t>(arg);
}

CounterQuality CounterQualitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return CounterQuality::ONLINE;
    case(0x2):
      return CounterQuality::RESTART;
    case(0x4):
      return CounterQuality::COMM_LOST;
    case(0x8):
      return CounterQuality::REMOTE_FORCED;
    case(0x10):
      return CounterQuality::LOCAL_FORCED;
    case(0x20):
      return CounterQuality::ROLLOVER;
    case(0x40):
      return CounterQuality::DISCONTINUITY;
    case(0x80):
      return CounterQuality::RESERVED;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* CounterQualitySpec::to_string(CounterQuality arg)
{
  switch(arg)
  {
    case(CounterQuality::ONLINE):
      return "ONLINE";
    case(CounterQuality::RESTART):
      return "RESTART";
    case(CounterQuality::COMM_LOST):
      return "COMM_LOST";
    case(CounterQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(CounterQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(CounterQuality::ROLLOVER):
      return "ROLLOVER";
    case(CounterQuality::DISCONTINUITY):
      return "DISCONTINUITY";
    case(CounterQuality::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

char const* CounterQualitySpec::to_human_string(CounterQuality arg)
{
  switch(arg)
  {
    case(CounterQuality::ONLINE):
      return "ONLINE";
    case(CounterQuality::RESTART):
      return "RESTART";
    case(CounterQuality::COMM_LOST):
      return "COMM_LOST";
    case(CounterQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(CounterQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(CounterQuality::ROLLOVER):
      return "ROLLOVER";
    case(CounterQuality::DISCONTINUITY):
      return "DISCONTINUITY";
    case(CounterQuality::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

CounterQuality CounterQualitySpec::from_string(const std::string& arg)
{
  if(arg == "ONLINE") return CounterQuality::ONLINE;
  if(arg == "RESTART") return CounterQuality::RESTART;
  if(arg == "COMM_LOST") return CounterQuality::COMM_LOST;
  if(arg == "REMOTE_FORCED") return CounterQuality::REMOTE_FORCED;
  if(arg == "LOCAL_FORCED") return CounterQuality::LOCAL_FORCED;
  if(arg == "ROLLOVER") return CounterQuality::ROLLOVER;
  if(arg == "DISCONTINUITY") return CounterQuality::DISCONTINUITY;
  if(arg == "RESERVED") return CounterQuality::RESERVED;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
