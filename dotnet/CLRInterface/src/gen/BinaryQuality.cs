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

namespace Automatak.DNP3.Interface
{
  /// <summary>
  /// Quality field bitmask for binary values
  /// </summary>
  public enum BinaryQuality : byte
  {
    /// <summary>
    /// set when the data is "good", meaning that rest of the system can trust the value
    /// </summary>
    ONLINE = 0x1,
    /// <summary>
    /// the quality all points get before we have established communication (or populated) the point
    /// </summary>
    RESTART = 0x2,
    /// <summary>
    /// set if communication has been lost with the source of the data (after establishing contact)
    /// </summary>
    COMM_LOST = 0x4,
    /// <summary>
    /// set if the value is being forced to a "fake" value somewhere in the system
    /// </summary>
    REMOTE_FORCED = 0x8,
    /// <summary>
    /// set if the value is being forced to a "fake" value on the original device
    /// </summary>
    LOCAL_FORCED = 0x10,
    /// <summary>
    /// set if the value is oscillating very quickly and some events are being suppressed
    /// </summary>
    CHATTER_FILTER = 0x20,
    /// <summary>
    /// reserved bit
    /// </summary>
    RESERVED = 0x40,
    /// <summary>
    /// state bit
    /// </summary>
    STATE = 0x80
  }
}
