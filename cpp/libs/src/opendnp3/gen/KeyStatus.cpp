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

#include "opendnp3/gen/KeyStatus.h"

namespace opendnp3 {

uint8_t KeyStatusToType(KeyStatus arg)
{
  return static_cast<uint8_t>(arg);
}
KeyStatus KeyStatusFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return KeyStatus::OK;
    case(0x2):
      return KeyStatus::NOT_INIT;
    case(0x3):
      return KeyStatus::COMM_FAIL;
    case(0x4):
      return KeyStatus::AUTH_FAIL;
    default:
      return KeyStatus::UNDEFINED;
  }
}
char const* KeyStatusToString(KeyStatus arg)
{
  switch(arg)
  {
    case(KeyStatus::OK):
      return "OK";
    case(KeyStatus::NOT_INIT):
      return "NOT_INIT";
    case(KeyStatus::COMM_FAIL):
      return "COMM_FAIL";
    case(KeyStatus::AUTH_FAIL):
      return "AUTH_FAIL";
    default:
      return "UNDEFINED";
  }
}

}
