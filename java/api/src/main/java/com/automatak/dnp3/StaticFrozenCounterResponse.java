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
// Copyright 2013 Automatak LLC
// 
// Automatak LLC (www.automatak.com) licenses this file
// to you under the the Apache License Version 2.0 (the "License"):
// 
// http://www.apache.org/licenses/LICENSE-2.0.html
//
package com.automatak.dnp3;

/**
*/
public enum StaticFrozenCounterResponse
{
  Group21Var1(0),
  Group21Var2(1),
  Group21Var5(2),
  Group21Var6(3),
  Group21Var7(4),
  Group21Var8(5),
  Group21Var9(6),
  Group21Var10(7);

  private final int id;

  private StaticFrozenCounterResponse(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static StaticFrozenCounterResponse fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return Group21Var1;
      case(1):
        return Group21Var2;
      case(2):
        return Group21Var5;
      case(3):
        return Group21Var6;
      case(4):
        return Group21Var7;
      case(5):
        return Group21Var8;
      case(6):
        return Group21Var9;
      case(7):
        return Group21Var10;
    }
    return Group21Var10;
  }
}
