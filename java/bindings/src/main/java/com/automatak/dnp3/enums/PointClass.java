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
* Class assignment for a measurement point
*/
public enum PointClass
{
  /**
  * No event class assignment
  */
  Class0(0x1),
  /**
  * Assigned to event class 1
  */
  Class1(0x2),
  /**
  * Assigned to event class 2
  */
  Class2(0x4),
  /**
  * Assigned to event class 3
  */
  Class3(0x8);

  private final int id;

  public int toType()
  {
    return id;
  }

  PointClass(int id)
  {
    this.id = id;
  }

  public static PointClass fromType(int arg)
  {
    switch(arg)
    {
      case(0x1):
        return Class0;
      case(0x2):
        return Class1;
      case(0x4):
        return Class2;
      case(0x8):
        return Class3;
      default:
        return Class0;
    }
  }
}
