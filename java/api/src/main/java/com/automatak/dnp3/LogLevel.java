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
* Enumeration for log levels
*/
public enum LogLevel
{
  Event(0x1),
  Error(0x2),
  Warning(0x4),
  Info(0x8),
  Interpret(0x10),
  Comm(0x20),
  Debug(0x40);

  private final int id;

  private LogLevel(int id)
  {
    this.id = id;
  }

  public int toType()
  {
    return id;
  }

  public static LogLevel fromType(int arg)
  {
    switch(arg)
    {
      case(0x1):
        return Event;
      case(0x2):
        return Error;
      case(0x4):
        return Warning;
      case(0x8):
        return Info;
      case(0x10):
        return Interpret;
      case(0x20):
        return Comm;
      case(0x40):
        return Debug;
    }
    return Debug;
  }
}
