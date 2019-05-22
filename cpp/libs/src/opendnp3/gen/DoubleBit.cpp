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
// Copyright 2013-2019 Automatak, LLC
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

#include "opendnp3/gen/DoubleBit.h"

namespace opendnp3 {

uint8_t DoubleBitToType(DoubleBit arg)
{
  return static_cast<uint8_t>(arg);
}
DoubleBit DoubleBitFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return DoubleBit::INTERMEDIATE;
    case(0x1):
      return DoubleBit::DETERMINED_OFF;
    case(0x2):
      return DoubleBit::DETERMINED_ON;
    default:
      return DoubleBit::INDETERMINATE;
  }
}
char const* DoubleBitToString(DoubleBit arg)
{
  switch(arg)
  {
    case(DoubleBit::INTERMEDIATE):
      return "INTERMEDIATE";
    case(DoubleBit::DETERMINED_OFF):
      return "DETERMINED_OFF";
    case(DoubleBit::DETERMINED_ON):
      return "DETERMINED_ON";
    default:
      return "INDETERMINATE";
  }
}

}
