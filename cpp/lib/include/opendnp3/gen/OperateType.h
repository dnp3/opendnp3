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

#ifndef OPENDNP3_OPERATETYPE_H
#define OPENDNP3_OPERATETYPE_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Various ways that an outstation can receive a request to operate a BO or AO point
*/
enum class OperateType : uint8_t
{
  /// The outstation received a valid prior SELECT followed by OPERATE
  SelectBeforeOperate = 0x0,
  /// The outstation received a direct operate request
  DirectOperate = 0x1,
  /// The outstation received a direct operate no ack request
  DirectOperateNoAck = 0x2
};

struct OperateTypeSpec
{
  using enum_type_t = OperateType;

  static uint8_t to_type(OperateType arg);
  static OperateType from_type(uint8_t arg);
  static char const* to_string(OperateType arg);
  static char const* to_human_string(OperateType arg);
  static OperateType from_string(const std::string& arg);
};

}

#endif
