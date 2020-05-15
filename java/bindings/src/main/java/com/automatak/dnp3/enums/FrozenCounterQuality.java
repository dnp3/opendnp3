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
* Quality field bitmask for frozen counter values
*/
public enum FrozenCounterQuality
{
  /**
  * set when the data is "good", meaning that rest of the system can trust the value
  */
  ONLINE(0x1),
  /**
  * the quality all points get before we have established communication (or populated) the point
  */
  RESTART(0x2),
  /**
  * set if communication has been lost with the source of the data (after establishing contact)
  */
  COMM_LOST(0x4),
  /**
  * set if the value is being forced to a "fake" value somewhere in the system
  */
  REMOTE_FORCED(0x8),
  /**
  * set if the value is being forced to a "fake" value on the original device
  */
  LOCAL_FORCED(0x10),
  /**
  * Deprecated flag that indicates value has rolled over
  */
  ROLLOVER(0x20),
  /**
  * indicates an unusual change in value
  */
  DISCONTINUITY(0x40),
  /**
  * reserved bit
  */
  RESERVED(0x80);

  private final int id;

  public int toType()
  {
    return id;
  }

  FrozenCounterQuality(int id)
  {
    this.id = id;
  }

  public static FrozenCounterQuality fromType(int arg)
  {
    switch(arg)
    {
      case(0x1):
        return ONLINE;
      case(0x2):
        return RESTART;
      case(0x4):
        return COMM_LOST;
      case(0x8):
        return REMOTE_FORCED;
      case(0x10):
        return LOCAL_FORCED;
      case(0x20):
        return ROLLOVER;
      case(0x40):
        return DISCONTINUITY;
      case(0x80):
        return RESERVED;
      default:
        return ONLINE;
    }
  }
}
