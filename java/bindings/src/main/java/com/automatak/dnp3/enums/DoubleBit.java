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
* Enumeration for possible states of a double bit value
*/
public enum DoubleBit
{
  /**
  * Transitioning between end conditions
  */
  INTERMEDIATE(0x0),
  /**
  * End condition, determined to be OFF
  */
  DETERMINED_OFF(0x1),
  /**
  * End condition, determined to be ON
  */
  DETERMINED_ON(0x2),
  /**
  * Abnormal or custom condition
  */
  INDETERMINATE(0x3);

  private final int id;

  public int toType()
  {
    return id;
  }

  DoubleBit(int id)
  {
    this.id = id;
  }

  public static DoubleBit fromType(int arg)
  {
    switch(arg)
    {
      case(0x0):
        return INTERMEDIATE;
      case(0x1):
        return DETERMINED_OFF;
      case(0x2):
        return DETERMINED_ON;
      default:
        return INDETERMINATE;
    }
  }
}
