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

#include "opendnp3/gen/LinkFunction.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t LinkFunctionSpec::to_type(LinkFunction arg)
{
  return static_cast<uint8_t>(arg);
}

LinkFunction LinkFunctionSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x40):
      return LinkFunction::PRI_RESET_LINK_STATES;
    case(0x42):
      return LinkFunction::PRI_TEST_LINK_STATES;
    case(0x43):
      return LinkFunction::PRI_CONFIRMED_USER_DATA;
    case(0x44):
      return LinkFunction::PRI_UNCONFIRMED_USER_DATA;
    case(0x49):
      return LinkFunction::PRI_REQUEST_LINK_STATUS;
    case(0x0):
      return LinkFunction::SEC_ACK;
    case(0x1):
      return LinkFunction::SEC_NACK;
    case(0xB):
      return LinkFunction::SEC_LINK_STATUS;
    case(0xF):
      return LinkFunction::SEC_NOT_SUPPORTED;
    default:
      return LinkFunction::INVALID;
  }
}

char const* LinkFunctionSpec::to_string(LinkFunction arg)
{
  switch(arg)
  {
    case(LinkFunction::PRI_RESET_LINK_STATES):
      return "PRI_RESET_LINK_STATES";
    case(LinkFunction::PRI_TEST_LINK_STATES):
      return "PRI_TEST_LINK_STATES";
    case(LinkFunction::PRI_CONFIRMED_USER_DATA):
      return "PRI_CONFIRMED_USER_DATA";
    case(LinkFunction::PRI_UNCONFIRMED_USER_DATA):
      return "PRI_UNCONFIRMED_USER_DATA";
    case(LinkFunction::PRI_REQUEST_LINK_STATUS):
      return "PRI_REQUEST_LINK_STATUS";
    case(LinkFunction::SEC_ACK):
      return "SEC_ACK";
    case(LinkFunction::SEC_NACK):
      return "SEC_NACK";
    case(LinkFunction::SEC_LINK_STATUS):
      return "SEC_LINK_STATUS";
    case(LinkFunction::SEC_NOT_SUPPORTED):
      return "SEC_NOT_SUPPORTED";
    default:
      return "INVALID";
  }
}

char const* LinkFunctionSpec::to_human_string(LinkFunction arg)
{
  switch(arg)
  {
    case(LinkFunction::PRI_RESET_LINK_STATES):
      return "PRI_RESET_LINK_STATES";
    case(LinkFunction::PRI_TEST_LINK_STATES):
      return "PRI_TEST_LINK_STATES";
    case(LinkFunction::PRI_CONFIRMED_USER_DATA):
      return "PRI_CONFIRMED_USER_DATA";
    case(LinkFunction::PRI_UNCONFIRMED_USER_DATA):
      return "PRI_UNCONFIRMED_USER_DATA";
    case(LinkFunction::PRI_REQUEST_LINK_STATUS):
      return "PRI_REQUEST_LINK_STATUS";
    case(LinkFunction::SEC_ACK):
      return "SEC_ACK";
    case(LinkFunction::SEC_NACK):
      return "SEC_NACK";
    case(LinkFunction::SEC_LINK_STATUS):
      return "SEC_LINK_STATUS";
    case(LinkFunction::SEC_NOT_SUPPORTED):
      return "SEC_NOT_SUPPORTED";
    default:
      return "INVALID";
  }
}

LinkFunction LinkFunctionSpec::from_string(const std::string& arg)
{
  if(arg == "PRI_RESET_LINK_STATES") return LinkFunction::PRI_RESET_LINK_STATES;
  if(arg == "PRI_TEST_LINK_STATES") return LinkFunction::PRI_TEST_LINK_STATES;
  if(arg == "PRI_CONFIRMED_USER_DATA") return LinkFunction::PRI_CONFIRMED_USER_DATA;
  if(arg == "PRI_UNCONFIRMED_USER_DATA") return LinkFunction::PRI_UNCONFIRMED_USER_DATA;
  if(arg == "PRI_REQUEST_LINK_STATUS") return LinkFunction::PRI_REQUEST_LINK_STATUS;
  if(arg == "SEC_ACK") return LinkFunction::SEC_ACK;
  if(arg == "SEC_NACK") return LinkFunction::SEC_NACK;
  if(arg == "SEC_LINK_STATUS") return LinkFunction::SEC_LINK_STATUS;
  if(arg == "SEC_NOT_SUPPORTED") return LinkFunction::SEC_NOT_SUPPORTED;
  else return LinkFunction::INVALID;
}


}
