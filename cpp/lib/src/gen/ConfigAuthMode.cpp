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

#include "opendnp3/gen/ConfigAuthMode.h"
#include <cstring>
#include <stdexcept>

namespace opendnp3 {

uint8_t ConfigAuthModeToType(ConfigAuthMode arg)
{
  return static_cast<uint8_t>(arg);
}
ConfigAuthMode ConfigAuthModeFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return ConfigAuthMode::NONE;
    case(0x1):
      return ConfigAuthMode::SAV5;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}
char const* ConfigAuthModeToString(ConfigAuthMode arg)
{
  switch(arg)
  {
    case(ConfigAuthMode::NONE):
      return "NONE";
    case(ConfigAuthMode::SAV5):
      return "SAV5";
    default:
      return "UNDEFINED";
  }
}
ConfigAuthMode ConfigAuthModeFromString(char const* arg)
{
  if(std::strncmp(arg, "NONE", 4)) return ConfigAuthMode::NONE;
  if(std::strncmp(arg, "SAV5", 4)) return ConfigAuthMode::SAV5;
  else throw new std::invalid_argument(std::string("Unknown value: ") + arg);
}

}
