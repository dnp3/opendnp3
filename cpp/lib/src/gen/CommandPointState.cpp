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

#include "opendnp3/gen/CommandPointState.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t CommandPointStateSpec::to_type(CommandPointState arg)
{
  return static_cast<uint8_t>(arg);
}

CommandPointState CommandPointStateSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return CommandPointState::INIT;
    case(1):
      return CommandPointState::SELECT_SUCCESS;
    case(2):
      return CommandPointState::SELECT_MISMATCH;
    case(3):
      return CommandPointState::SELECT_FAIL;
    case(4):
      return CommandPointState::OPERATE_FAIL;
    case(5):
      return CommandPointState::SUCCESS;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}

char const* CommandPointStateSpec::to_string(CommandPointState arg)
{
  switch(arg)
  {
    case(CommandPointState::INIT):
      return "INIT";
    case(CommandPointState::SELECT_SUCCESS):
      return "SELECT_SUCCESS";
    case(CommandPointState::SELECT_MISMATCH):
      return "SELECT_MISMATCH";
    case(CommandPointState::SELECT_FAIL):
      return "SELECT_FAIL";
    case(CommandPointState::OPERATE_FAIL):
      return "OPERATE_FAIL";
    case(CommandPointState::SUCCESS):
      return "SUCCESS";
    default:
      return "UNDEFINED";
  }
}

char const* CommandPointStateSpec::to_human_string(CommandPointState arg)
{
  switch(arg)
  {
    case(CommandPointState::INIT):
      return "INIT";
    case(CommandPointState::SELECT_SUCCESS):
      return "SELECT_SUCCESS";
    case(CommandPointState::SELECT_MISMATCH):
      return "SELECT_MISMATCH";
    case(CommandPointState::SELECT_FAIL):
      return "SELECT_FAIL";
    case(CommandPointState::OPERATE_FAIL):
      return "OPERATE_FAIL";
    case(CommandPointState::SUCCESS):
      return "SUCCESS";
    default:
      return "UNDEFINED";
  }
}

CommandPointState CommandPointStateSpec::from_string(const std::string& arg)
{
  if(arg == "INIT") return CommandPointState::INIT;
  if(arg == "SELECT_SUCCESS") return CommandPointState::SELECT_SUCCESS;
  if(arg == "SELECT_MISMATCH") return CommandPointState::SELECT_MISMATCH;
  if(arg == "SELECT_FAIL") return CommandPointState::SELECT_FAIL;
  if(arg == "OPERATE_FAIL") return CommandPointState::OPERATE_FAIL;
  if(arg == "SUCCESS") return CommandPointState::SUCCESS;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}


}
