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

#ifndef OPENDNP3_BINARYOUTPUTSTATUSQUALITY_H
#define OPENDNP3_BINARYOUTPUTSTATUSQUALITY_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Quality field bitmask for binary output status values
*/
enum class BinaryOutputStatusQuality : uint8_t
{
  /// set when the data is "good", meaning that rest of the system can trust the value
  ONLINE = 0x1,
  /// the quality all points get before we have established communication (or populated) the point
  RESTART = 0x2,
  /// set if communication has been lost with the source of the data (after establishing contact)
  COMM_LOST = 0x4,
  /// set if the value is being forced to a "fake" value somewhere in the system
  REMOTE_FORCED = 0x8,
  /// set if the value is being forced to a "fake" value on the original device
  LOCAL_FORCED = 0x10,
  /// reserved bit 1
  RESERVED1 = 0x20,
  /// reserved bit 2
  RESERVED2 = 0x40,
  /// state bit
  STATE = 0x80
};

struct BinaryOutputStatusQualitySpec
{
  using enum_type_t = BinaryOutputStatusQuality;

  static uint8_t to_type(BinaryOutputStatusQuality arg);
  static BinaryOutputStatusQuality from_type(uint8_t arg);
  static char const* to_string(BinaryOutputStatusQuality arg);
  static char const* to_human_string(BinaryOutputStatusQuality arg);
  static BinaryOutputStatusQuality from_string(const std::string& arg);
};

}

#endif
