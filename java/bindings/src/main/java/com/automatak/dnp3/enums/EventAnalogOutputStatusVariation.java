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
*/
public enum EventAnalogOutputStatusVariation
{
  Group42Var1(0),
  Group42Var2(1),
  Group42Var3(2),
  Group42Var4(3),
  Group42Var5(4),
  Group42Var6(5),
  Group42Var7(6),
  Group42Var8(7);

  private final int id;

  public int toType()
  {
    return id;
  }

  EventAnalogOutputStatusVariation(int id)
  {
    this.id = id;
  }

  public static EventAnalogOutputStatusVariation fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return Group42Var1;
      case(1):
        return Group42Var2;
      case(2):
        return Group42Var3;
      case(3):
        return Group42Var4;
      case(4):
        return Group42Var5;
      case(5):
        return Group42Var6;
      case(6):
        return Group42Var7;
      case(7):
        return Group42Var8;
      default:
        return Group42Var1;
    }
  }
}
