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

#ifndef OPENDNP3_LINKFUNCTION_H
#define OPENDNP3_LINKFUNCTION_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Link layer function code enumeration
*/
enum class LinkFunction : uint8_t
{
  PRI_RESET_LINK_STATES = 0x40,
  PRI_TEST_LINK_STATES = 0x42,
  PRI_CONFIRMED_USER_DATA = 0x43,
  PRI_UNCONFIRMED_USER_DATA = 0x44,
  PRI_REQUEST_LINK_STATUS = 0x49,
  SEC_ACK = 0x0,
  SEC_NACK = 0x1,
  SEC_LINK_STATUS = 0xB,
  SEC_NOT_SUPPORTED = 0xF,
  INVALID = 0xFF
};

struct LinkFunctionSpec
{
  using enum_type_t = LinkFunction;

  static uint8_t to_type(LinkFunction arg);
  static LinkFunction from_type(uint8_t arg);
  static char const* to_string(LinkFunction arg);
  static char const* to_human_string(LinkFunction arg);
  static LinkFunction from_string(const std::string& arg);
};

}

#endif
