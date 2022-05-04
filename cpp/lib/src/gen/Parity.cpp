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

#include "opendnp3/gen/Parity.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t ParitySpec::to_type(Parity arg)
{
  return static_cast<uint8_t>(arg);
}

Parity ParitySpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(1):
      return Parity::Even;
    case(2):
      return Parity::Odd;
    default:
      return Parity::None;
  }
}

char const* ParitySpec::to_string(Parity arg)
{
  switch(arg)
  {
    case(Parity::Even):
      return "Even";
    case(Parity::Odd):
      return "Odd";
    default:
      return "None";
  }
}

char const* ParitySpec::to_human_string(Parity arg)
{
  switch(arg)
  {
    case(Parity::Even):
      return "Even";
    case(Parity::Odd):
      return "Odd";
    default:
      return "None";
  }
}

Parity ParitySpec::from_string(const std::string& arg)
{
  if(arg == "Even") return Parity::Even;
  if(arg == "Odd") return Parity::Odd;
  else return Parity::None;
}


}
