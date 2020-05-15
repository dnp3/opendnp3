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

#include "opendnp3/gen/RestartMode.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t RestartModeSpec::to_type(RestartMode arg)
{
  return static_cast<uint8_t>(arg);
}

RestartMode RestartModeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return RestartMode::UNSUPPORTED;
    case(1):
      return RestartMode::SUPPORTED_DELAY_FINE;
    case(2):
      return RestartMode::SUPPORTED_DELAY_COARSE;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* RestartModeSpec::to_string(RestartMode arg)
{
  switch(arg)
  {
    case(RestartMode::UNSUPPORTED):
      return "UNSUPPORTED";
    case(RestartMode::SUPPORTED_DELAY_FINE):
      return "SUPPORTED_DELAY_FINE";
    case(RestartMode::SUPPORTED_DELAY_COARSE):
      return "SUPPORTED_DELAY_COARSE";
    default:
      return "UNDEFINED";
  }
}

char const* RestartModeSpec::to_human_string(RestartMode arg)
{
  switch(arg)
  {
    case(RestartMode::UNSUPPORTED):
      return "UNSUPPORTED";
    case(RestartMode::SUPPORTED_DELAY_FINE):
      return "SUPPORTED_DELAY_FINE";
    case(RestartMode::SUPPORTED_DELAY_COARSE):
      return "SUPPORTED_DELAY_COARSE";
    default:
      return "UNDEFINED";
  }
}

RestartMode RestartModeSpec::from_string(const std::string& arg)
{
  if(arg == "UNSUPPORTED") return RestartMode::UNSUPPORTED;
  if(arg == "SUPPORTED_DELAY_FINE") return RestartMode::SUPPORTED_DELAY_FINE;
  if(arg == "SUPPORTED_DELAY_COARSE") return RestartMode::SUPPORTED_DELAY_COARSE;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
