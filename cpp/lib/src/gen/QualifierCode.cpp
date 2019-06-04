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

#include "opendnp3/gen/QualifierCode.h"

namespace opendnp3 {

uint8_t QualifierCodeToType(QualifierCode arg)
{
  return static_cast<uint8_t>(arg);
}
QualifierCode QualifierCodeFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return QualifierCode::UINT8_START_STOP;
    case(0x1):
      return QualifierCode::UINT16_START_STOP;
    case(0x6):
      return QualifierCode::ALL_OBJECTS;
    case(0x7):
      return QualifierCode::UINT8_CNT;
    case(0x8):
      return QualifierCode::UINT16_CNT;
    case(0x17):
      return QualifierCode::UINT8_CNT_UINT8_INDEX;
    case(0x28):
      return QualifierCode::UINT16_CNT_UINT16_INDEX;
    case(0x5B):
      return QualifierCode::UINT16_FREE_FORMAT;
    default:
      return QualifierCode::UNDEFINED;
  }
}
char const* QualifierCodeToString(QualifierCode arg)
{
  switch(arg)
  {
    case(QualifierCode::UINT8_START_STOP):
      return "8-bit start stop";
    case(QualifierCode::UINT16_START_STOP):
      return "16-bit start stop";
    case(QualifierCode::ALL_OBJECTS):
      return "all objects";
    case(QualifierCode::UINT8_CNT):
      return "8-bit count";
    case(QualifierCode::UINT16_CNT):
      return "16-bit count";
    case(QualifierCode::UINT8_CNT_UINT8_INDEX):
      return "8-bit count and prefix";
    case(QualifierCode::UINT16_CNT_UINT16_INDEX):
      return "16-bit count and prefix";
    case(QualifierCode::UINT16_FREE_FORMAT):
      return "16-bit free format";
    default:
      return "unknown";
  }
}

}
