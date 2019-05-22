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

#include "opendnp3/gen/UserRole.h"

namespace opendnp3 {

uint16_t UserRoleToType(UserRole arg)
{
  return static_cast<uint16_t>(arg);
}
UserRole UserRoleFromType(uint16_t arg)
{
  switch(arg)
  {
    case(0):
      return UserRole::VIEWER;
    case(1):
      return UserRole::OPERATOR;
    case(2):
      return UserRole::ENGINEER;
    case(3):
      return UserRole::INSTALLER;
    case(4):
      return UserRole::SECADM;
    case(5):
      return UserRole::SECAUD;
    case(6):
      return UserRole::RBACMNT;
    case(32768):
      return UserRole::SINGLE_USER;
    default:
      return UserRole::UNDEFINED;
  }
}
char const* UserRoleToString(UserRole arg)
{
  switch(arg)
  {
    case(UserRole::VIEWER):
      return "VIEWER";
    case(UserRole::OPERATOR):
      return "OPERATOR";
    case(UserRole::ENGINEER):
      return "ENGINEER";
    case(UserRole::INSTALLER):
      return "INSTALLER";
    case(UserRole::SECADM):
      return "SECADM";
    case(UserRole::SECAUD):
      return "SECAUD";
    case(UserRole::RBACMNT):
      return "RBACMNT";
    case(UserRole::SINGLE_USER):
      return "SINGLE_USER";
    default:
      return "UNDEFINED";
  }
}

}
