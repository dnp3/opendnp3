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

package com.automatak.dnp3.enums;
/**
* groups that can be used inconjunction with the ASSIGN_CLASS function code
*/
public enum AssignClassType
{
  BinaryInput(0x0),
  DoubleBinaryInput(0x1),
  Counter(0x2),
  FrozenCounter(0x3),
  AnalogInput(0x4),
  BinaryOutputStatus(0x5),
  AnalogOutputStatus(0x6);

  private final int id;

  public int toType()
  {
    return id;
  }

  AssignClassType(int id)
  {
    this.id = id;
  }
}
