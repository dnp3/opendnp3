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

#include "opendnp3/gen/FlowControl.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t FlowControlSpec::to_type(FlowControl arg)
{
  return static_cast<uint8_t>(arg);
}

FlowControl FlowControlSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(1):
      return FlowControl::Hardware;
    case(2):
      return FlowControl::XONXOFF;
    default:
      return FlowControl::None;
  }
}

char const* FlowControlSpec::to_string(FlowControl arg)
{
  switch(arg)
  {
    case(FlowControl::Hardware):
      return "Hardware";
    case(FlowControl::XONXOFF):
      return "XONXOFF";
    default:
      return "None";
  }
}

char const* FlowControlSpec::to_human_string(FlowControl arg)
{
  switch(arg)
  {
    case(FlowControl::Hardware):
      return "Hardware";
    case(FlowControl::XONXOFF):
      return "XONXOFF";
    default:
      return "None";
  }
}

FlowControl FlowControlSpec::from_string(const std::string& arg)
{
  if(arg == "Hardware") return FlowControl::Hardware;
  if(arg == "XONXOFF") return FlowControl::XONXOFF;
  else return FlowControl::None;
}


}
