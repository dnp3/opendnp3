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
// LLC (www.automatak.com) under one or more contributor license agreements.
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
*/
public enum EventAnalogVariation
{
  Group32Var1(0),
  Group32Var2(1),
  Group32Var3(2),
  Group32Var4(3),
  Group32Var5(4),
  Group32Var6(5),
  Group32Var7(6),
  Group32Var8(7);

  private final int id;

  public int toType()
  {
    return id;
  }

  EventAnalogVariation(int id)
  {
    this.id = id;
  }

  public static EventAnalogVariation fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return Group32Var1;
      case(1):
        return Group32Var2;
      case(2):
        return Group32Var3;
      case(3):
        return Group32Var4;
      case(4):
        return Group32Var5;
      case(5):
        return Group32Var6;
      case(6):
        return Group32Var7;
      case(7):
        return Group32Var8;
      default:
        return Group32Var1;
    }
  }
}
