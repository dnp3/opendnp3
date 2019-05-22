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

package com.automatak.dnp3.enums;
/**
* Enumerates pre-defined rules in secure authentication
*/
public enum UserRole
{
  VIEWER(0),
  OPERATOR(1),
  ENGINEER(2),
  INSTALLER(3),
  SECADM(4),
  SECAUD(5),
  RBACMNT(6),
  SINGLE_USER(32768),
  UNDEFINED(32767);

  private final int id;

  public int toType()
  {
    return id;
  }

  UserRole(int id)
  {
    this.id = id;
  }

  public static UserRole fromType(int arg)
  {
    switch(arg)
    {
      case(0):
        return VIEWER;
      case(1):
        return OPERATOR;
      case(2):
        return ENGINEER;
      case(3):
        return INSTALLER;
      case(4):
        return SECADM;
      case(5):
        return SECAUD;
      case(6):
        return RBACMNT;
      case(32768):
        return SINGLE_USER;
      default:
        return UNDEFINED;
    }
  }
}
