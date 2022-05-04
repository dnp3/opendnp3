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

#ifndef OPENDNP3_ANALOGOUTPUTSTATUSQUALITY_H
#define OPENDNP3_ANALOGOUTPUTSTATUSQUALITY_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Quality field bitmask for analog output status values
*/
enum class AnalogOutputStatusQuality : uint8_t
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
  /// set if a hardware input etc. is out of range and we are using a place holder value
  OVERRANGE = 0x20,
  /// set if calibration or reference voltage has been lost meaning readings are questionable
  REFERENCE_ERR = 0x40,
  /// reserved bit
  RESERVED = 0x80
};

struct AnalogOutputStatusQualitySpec
{
  using enum_type_t = AnalogOutputStatusQuality;

  static uint8_t to_type(AnalogOutputStatusQuality arg);
  static AnalogOutputStatusQuality from_type(uint8_t arg);
  static char const* to_string(AnalogOutputStatusQuality arg);
  static char const* to_human_string(AnalogOutputStatusQuality arg);
  static AnalogOutputStatusQuality from_string(const std::string& arg);
};

}

#endif
