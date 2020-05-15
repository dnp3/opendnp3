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

#ifndef OPENDNP3_QUALIFIERCODE_H
#define OPENDNP3_QUALIFIERCODE_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Object header range/prefix as a single enumeration
*/
enum class QualifierCode : uint8_t
{
  UINT8_START_STOP = 0x0,
  UINT16_START_STOP = 0x1,
  ALL_OBJECTS = 0x6,
  UINT8_CNT = 0x7,
  UINT16_CNT = 0x8,
  UINT8_CNT_UINT8_INDEX = 0x17,
  UINT16_CNT_UINT16_INDEX = 0x28,
  UNDEFINED = 0xFF
};

struct QualifierCodeSpec
{
  using enum_type_t = QualifierCode;

  static uint8_t to_type(QualifierCode arg);
  static QualifierCode from_type(uint8_t arg);
  static char const* to_string(QualifierCode arg);
  static char const* to_human_string(QualifierCode arg);
  static QualifierCode from_string(const std::string& arg);
};

}

#endif
