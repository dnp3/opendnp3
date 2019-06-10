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
// Copyright 2013-2019 Automatak, LLC
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

#include "opendnp3/gen/FrozenCounterQuality.h"

namespace opendnp3 {

uint8_t FrozenCounterQualityToType(FrozenCounterQuality arg)
{
  return static_cast<uint8_t>(arg);
}
FrozenCounterQuality FrozenCounterQualityFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return FrozenCounterQuality::ONLINE;
    case(0x2):
      return FrozenCounterQuality::RESTART;
    case(0x4):
      return FrozenCounterQuality::COMM_LOST;
    case(0x8):
      return FrozenCounterQuality::REMOTE_FORCED;
    case(0x10):
      return FrozenCounterQuality::LOCAL_FORCED;
    case(0x20):
      return FrozenCounterQuality::ROLLOVER;
    case(0x40):
      return FrozenCounterQuality::DISCONTINUITY;
    case(0x80):
      return FrozenCounterQuality::RESERVED;
  }
}
char const* FrozenCounterQualityToString(FrozenCounterQuality arg)
{
  switch(arg)
  {
    case(FrozenCounterQuality::ONLINE):
      return "ONLINE";
    case(FrozenCounterQuality::RESTART):
      return "RESTART";
    case(FrozenCounterQuality::COMM_LOST):
      return "COMM_LOST";
    case(FrozenCounterQuality::REMOTE_FORCED):
      return "REMOTE_FORCED";
    case(FrozenCounterQuality::LOCAL_FORCED):
      return "LOCAL_FORCED";
    case(FrozenCounterQuality::ROLLOVER):
      return "ROLLOVER";
    case(FrozenCounterQuality::DISCONTINUITY):
      return "DISCONTINUITY";
    case(FrozenCounterQuality::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

}
