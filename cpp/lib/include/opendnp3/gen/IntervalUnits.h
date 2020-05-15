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

#ifndef OPENDNP3_INTERVALUNITS_H
#define OPENDNP3_INTERVALUNITS_H

#include <cstdint>
#include <string>

namespace opendnp3 {

/**
  Time internal units
*/
enum class IntervalUnits : uint8_t
{
  /// The outstation does not repeat the action regardless of the value in the interval count
  NoRepeat = 0x0,
  /// the interval is always counted relative to the start time and is constant regardless of the clock time set at the outstation
  Milliseconds = 0x1,
  /// At the same millisecond within the second that is specified in the start time
  Seconds = 0x2,
  /// At the same second and millisecond within the minute that is specified in the start time
  Minutes = 0x3,
  /// At the same minute, second and millisecond within the hour that is specified in the start time
  Hours = 0x4,
  /// At the same time of day that is specified in the start time
  Days = 0x5,
  /// On the same day of the week at the same time of day that is specified in the start time
  Weeks = 0x6,
  /// On the same day of each month at the same time of day that is specified in the start time
  Months7 = 0x7,
  /// At the same time of day on the same day of the week after the beginning of the month as the day specified in the start time
  Months8 = 0x8,
  /// Months on Same Day of Week from End of Month - The outstation shall interpret this setting as in Months8, but the day of the week shall be measured from the end of the month,
  Months9 = 0x9,
  /// The definition of a season is specific to the outstation
  Seasons = 0xA,
  /// 11-127 are reserved for future use
  Undefined = 0x7F
};

struct IntervalUnitsSpec
{
  using enum_type_t = IntervalUnits;

  static uint8_t to_type(IntervalUnits arg);
  static IntervalUnits from_type(uint8_t arg);
  static char const* to_string(IntervalUnits arg);
  static char const* to_human_string(IntervalUnits arg);
  static IntervalUnits from_string(const std::string& arg);
};

}

#endif
