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

#ifndef OPENDNP3_RESTARTMODE_H
#define OPENDNP3_RESTARTMODE_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Enumeration describing restart mode support of an outstation
*/
enum class RestartMode : uint8_t
{
  /// Device does not support restart
  UNSUPPORTED = 0,
  /// Supports restart, and time returned is a fine time delay
  SUPPORTED_DELAY_FINE = 1,
  /// Supports restart, and time returned is a coarse time delay
  SUPPORTED_DELAY_COARSE = 2
};

struct RestartModeSpec
{
  using enum_type_t = RestartMode;

  static uint8_t to_type(RestartMode arg);
  static RestartMode from_type(uint8_t arg);
  static char const* to_string(RestartMode arg);
  static char const* to_human_string(RestartMode arg);
  static RestartMode from_string(const std::string& arg);
};

}

#endif
