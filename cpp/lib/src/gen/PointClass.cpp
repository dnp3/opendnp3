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

#include "opendnp3/gen/PointClass.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t PointClassSpec::to_type(PointClass arg)
{
  return static_cast<uint8_t>(arg);
}

PointClass PointClassSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return PointClass::Class0;
    case(0x2):
      return PointClass::Class1;
    case(0x4):
      return PointClass::Class2;
    case(0x8):
      return PointClass::Class3;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* PointClassSpec::to_string(PointClass arg)
{
  switch(arg)
  {
    case(PointClass::Class0):
      return "Class0";
    case(PointClass::Class1):
      return "Class1";
    case(PointClass::Class2):
      return "Class2";
    case(PointClass::Class3):
      return "Class3";
    default:
      return "UNDEFINED";
  }
}

char const* PointClassSpec::to_human_string(PointClass arg)
{
  switch(arg)
  {
    case(PointClass::Class0):
      return "Class0";
    case(PointClass::Class1):
      return "Class1";
    case(PointClass::Class2):
      return "Class2";
    case(PointClass::Class3):
      return "Class3";
    default:
      return "UNDEFINED";
  }
}

PointClass PointClassSpec::from_string(const std::string& arg)
{
  if(arg == "Class0") return PointClass::Class0;
  if(arg == "Class1") return PointClass::Class1;
  if(arg == "Class2") return PointClass::Class2;
  if(arg == "Class3") return PointClass::Class3;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
