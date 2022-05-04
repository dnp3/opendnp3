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
// Copyright 2013-2022 Step Function I/O, LLC
// 
// Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
// LLC (https://stepfunc.io) under one or more contributor license agreements.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

package com.automatak.dnp3.enums;
/**
* Object header range/prefix as a single enumeration
*/
public enum QualifierCode
{
  UINT8_START_STOP(0x0),
  UINT16_START_STOP(0x1),
  ALL_OBJECTS(0x6),
  UINT8_CNT(0x7),
  UINT16_CNT(0x8),
  UINT8_CNT_UINT8_INDEX(0x17),
  UINT16_CNT_UINT16_INDEX(0x28),
  UNDEFINED(0xFF);

  private final int id;

  public int toType()
  {
    return id;
  }

  QualifierCode(int id)
  {
    this.id = id;
  }

  public static QualifierCode fromType(int arg)
  {
    switch(arg)
    {
      case(0x0):
        return UINT8_START_STOP;
      case(0x1):
        return UINT16_START_STOP;
      case(0x6):
        return ALL_OBJECTS;
      case(0x7):
        return UINT8_CNT;
      case(0x8):
        return UINT16_CNT;
      case(0x17):
        return UINT8_CNT_UINT8_INDEX;
      case(0x28):
        return UINT16_CNT_UINT16_INDEX;
      default:
        return UNDEFINED;
    }
  }
}
