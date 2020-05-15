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

#include "opendnp3/gen/TripCloseCode.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t TripCloseCodeSpec::to_type(TripCloseCode arg)
{
  return static_cast<uint8_t>(arg);
}

TripCloseCode TripCloseCodeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return TripCloseCode::NUL;
    case(0x1):
      return TripCloseCode::CLOSE;
    case(0x2):
      return TripCloseCode::TRIP;
    case(0x3):
      return TripCloseCode::RESERVED;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* TripCloseCodeSpec::to_string(TripCloseCode arg)
{
  switch(arg)
  {
    case(TripCloseCode::NUL):
      return "NUL";
    case(TripCloseCode::CLOSE):
      return "CLOSE";
    case(TripCloseCode::TRIP):
      return "TRIP";
    case(TripCloseCode::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

char const* TripCloseCodeSpec::to_human_string(TripCloseCode arg)
{
  switch(arg)
  {
    case(TripCloseCode::NUL):
      return "NUL";
    case(TripCloseCode::CLOSE):
      return "CLOSE";
    case(TripCloseCode::TRIP):
      return "TRIP";
    case(TripCloseCode::RESERVED):
      return "RESERVED";
    default:
      return "UNDEFINED";
  }
}

TripCloseCode TripCloseCodeSpec::from_string(const std::string& arg)
{
  if(arg == "NUL") return TripCloseCode::NUL;
  if(arg == "CLOSE") return TripCloseCode::CLOSE;
  if(arg == "TRIP") return TripCloseCode::TRIP;
  if(arg == "RESERVED") return TripCloseCode::RESERVED;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
