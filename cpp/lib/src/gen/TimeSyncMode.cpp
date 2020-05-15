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

#include "opendnp3/gen/TimeSyncMode.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t TimeSyncModeSpec::to_type(TimeSyncMode arg)
{
  return static_cast<uint8_t>(arg);
}

TimeSyncMode TimeSyncModeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(1):
      return TimeSyncMode::NonLAN;
    case(2):
      return TimeSyncMode::LAN;
    default:
      return TimeSyncMode::None;
  }
}

char const* TimeSyncModeSpec::to_string(TimeSyncMode arg)
{
  switch(arg)
  {
    case(TimeSyncMode::NonLAN):
      return "NonLAN";
    case(TimeSyncMode::LAN):
      return "LAN";
    default:
      return "None";
  }
}

char const* TimeSyncModeSpec::to_human_string(TimeSyncMode arg)
{
  switch(arg)
  {
    case(TimeSyncMode::NonLAN):
      return "NonLAN";
    case(TimeSyncMode::LAN):
      return "LAN";
    default:
      return "None";
  }
}

TimeSyncMode TimeSyncModeSpec::from_string(const std::string& arg)
{
  if(arg == "NonLAN") return TimeSyncMode::NonLAN;
  if(arg == "LAN") return TimeSyncMode::LAN;
  else return TimeSyncMode::None;
}


}
