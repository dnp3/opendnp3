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
* Link layer function code enumeration
*/
public enum LinkFunction
{
  PRI_RESET_LINK_STATES(0x40),
  PRI_TEST_LINK_STATES(0x42),
  PRI_CONFIRMED_USER_DATA(0x43),
  PRI_UNCONFIRMED_USER_DATA(0x44),
  PRI_REQUEST_LINK_STATUS(0x49),
  SEC_ACK(0x0),
  SEC_NACK(0x1),
  SEC_LINK_STATUS(0xB),
  SEC_NOT_SUPPORTED(0xF),
  INVALID(0xFF);

  private final int id;

  public int toType()
  {
    return id;
  }

  LinkFunction(int id)
  {
    this.id = id;
  }

  public static LinkFunction fromType(int arg)
  {
    switch(arg)
    {
      case(0x40):
        return PRI_RESET_LINK_STATES;
      case(0x42):
        return PRI_TEST_LINK_STATES;
      case(0x43):
        return PRI_CONFIRMED_USER_DATA;
      case(0x44):
        return PRI_UNCONFIRMED_USER_DATA;
      case(0x49):
        return PRI_REQUEST_LINK_STATUS;
      case(0x0):
        return SEC_ACK;
      case(0x1):
        return SEC_NACK;
      case(0xB):
        return SEC_LINK_STATUS;
      case(0xF):
        return SEC_NOT_SUPPORTED;
      default:
        return INVALID;
    }
  }
}
