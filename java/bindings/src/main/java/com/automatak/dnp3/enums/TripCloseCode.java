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

package com.automatak.dnp3.enums;
/**
* Used in conjunction with Operation Type in a CROB to describe which output to operate for complementary two-output model
* Refer to section A.8.1 of IEEE 1815-2012 for a full description
*/
public enum TripCloseCode
{
  /**
  * Use the default output.
  */
  NUL(0x0),
  /**
  * For complementary two-output model, operate the close output.
  */
  CLOSE(0x1),
  /**
  * For complementary two-output model, operate the trip output.
  */
  TRIP(0x2),
  /**
  * Reserved for future use.
  */
  RESERVED(0x3);

  private final int id;

  public int toType()
  {
    return id;
  }

  TripCloseCode(int id)
  {
    this.id = id;
  }

  public static TripCloseCode fromType(int arg)
  {
    switch(arg)
    {
      case(0x0):
        return NUL;
      case(0x1):
        return CLOSE;
      case(0x2):
        return TRIP;
      case(0x3):
        return RESERVED;
      default:
        return NUL;
    }
  }
}
