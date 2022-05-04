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
* Indicates the quality of timestamp values
*/
public enum TimestampQuality
{
  /**
  * The timestamp is UTC synchronized at the remote device
  */
  SYNCHRONIZED(1),
  /**
  * The device indicate the timestamp may be unsynchronized
  */
  UNSYNCHRONIZED(2),
  /**
  * Timestamp is not valid, ignore the value and use a local timestamp
  */
  INVALID(0);

  private final int id;

  public int toType()
  {
    return id;
  }

  TimestampQuality(int id)
  {
    this.id = id;
  }

  public static TimestampQuality fromType(int arg)
  {
    switch(arg)
    {
      case(1):
        return SYNCHRONIZED;
      case(2):
        return UNSYNCHRONIZED;
      default:
        return INVALID;
    }
  }
}
