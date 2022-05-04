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

namespace Automatak.DNP3.Interface
{
  /// <summary>
  /// Used in conjunction with Trip Close Code in a CROB to describe what action to perform
  /// Refer to section A.8.1 of IEEE 1815-2012 for a full description
  /// </summary>
  public enum OperationType : byte
  {
    /// <summary>
    /// Do nothing.
    /// </summary>
    NUL = 0x0,
    /// <summary>
    /// Set output to active for the duration of the On-time.
    /// </summary>
    PULSE_ON = 0x1,
    /// <summary>
    /// Non-interoperable code. Do not use for new applications.
    /// </summary>
    PULSE_OFF = 0x2,
    /// <summary>
    /// Set output to active.
    /// </summary>
    LATCH_ON = 0x3,
    /// <summary>
    /// Set the output to inactive.
    /// </summary>
    LATCH_OFF = 0x4,
    /// <summary>
    /// Undefined.
    /// </summary>
    Undefined = 0xFF
  }
}
