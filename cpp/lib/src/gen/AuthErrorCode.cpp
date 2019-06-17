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

#include "opendnp3/gen/AuthErrorCode.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t AuthErrorCodeSpec::to_type(AuthErrorCode arg)
{
  return static_cast<uint8_t>(arg);
}

AuthErrorCode AuthErrorCodeSpec::from_type(uint8_t arg)
{
  switch(arg)
  {
    case(0x1):
      return AuthErrorCode::AUTHENTICATION_FAILED;
    case(0x4):
      return AuthErrorCode::AGGRESSIVE_MODE_UNSUPPORTED;
    case(0x5):
      return AuthErrorCode::MAC_NOT_SUPPORTED;
    case(0x6):
      return AuthErrorCode::KEY_WRAP_NOT_SUPPORTED;
    case(0x7):
      return AuthErrorCode::AUTHORIZATION_FAILED;
    case(0x8):
      return AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED;
    case(0x9):
      return AuthErrorCode::INVALID_SIGNATURE;
    case(0xA):
      return AuthErrorCode::INVALID_CERTIFICATION_DATA;
    case(0xB):
      return AuthErrorCode::UNKNOWN_USER;
    case(0xC):
      return AuthErrorCode::MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED;
    default:
      return AuthErrorCode::UNKNOWN;
  }
}

char const* AuthErrorCodeSpec::to_string(AuthErrorCode arg)
{
  switch(arg)
  {
    case(AuthErrorCode::AUTHENTICATION_FAILED):
      return "AUTHENTICATION_FAILED";
    case(AuthErrorCode::AGGRESSIVE_MODE_UNSUPPORTED):
      return "AGGRESSIVE_MODE_UNSUPPORTED";
    case(AuthErrorCode::MAC_NOT_SUPPORTED):
      return "MAC_NOT_SUPPORTED";
    case(AuthErrorCode::KEY_WRAP_NOT_SUPPORTED):
      return "KEY_WRAP_NOT_SUPPORTED";
    case(AuthErrorCode::AUTHORIZATION_FAILED):
      return "AUTHORIZATION_FAILED";
    case(AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED):
      return "UPDATE_KEY_METHOD_NOT_PERMITTED";
    case(AuthErrorCode::INVALID_SIGNATURE):
      return "INVALID_SIGNATURE";
    case(AuthErrorCode::INVALID_CERTIFICATION_DATA):
      return "INVALID_CERTIFICATION_DATA";
    case(AuthErrorCode::UNKNOWN_USER):
      return "UNKNOWN_USER";
    case(AuthErrorCode::MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED):
      return "MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED";
    default:
      return "UNKNOWN";
  }
}

char const* AuthErrorCodeSpec::to_human_string(AuthErrorCode arg)
{
  switch(arg)
  {
    case(AuthErrorCode::AUTHENTICATION_FAILED):
      return "AUTHENTICATION_FAILED";
    case(AuthErrorCode::AGGRESSIVE_MODE_UNSUPPORTED):
      return "AGGRESSIVE_MODE_UNSUPPORTED";
    case(AuthErrorCode::MAC_NOT_SUPPORTED):
      return "MAC_NOT_SUPPORTED";
    case(AuthErrorCode::KEY_WRAP_NOT_SUPPORTED):
      return "KEY_WRAP_NOT_SUPPORTED";
    case(AuthErrorCode::AUTHORIZATION_FAILED):
      return "AUTHORIZATION_FAILED";
    case(AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED):
      return "UPDATE_KEY_METHOD_NOT_PERMITTED";
    case(AuthErrorCode::INVALID_SIGNATURE):
      return "INVALID_SIGNATURE";
    case(AuthErrorCode::INVALID_CERTIFICATION_DATA):
      return "INVALID_CERTIFICATION_DATA";
    case(AuthErrorCode::UNKNOWN_USER):
      return "UNKNOWN_USER";
    case(AuthErrorCode::MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED):
      return "MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED";
    default:
      return "UNKNOWN";
  }
}

AuthErrorCode AuthErrorCodeSpec::from_string(const std::string& arg)
{
  if(arg == "AUTHENTICATION_FAILED") return AuthErrorCode::AUTHENTICATION_FAILED;
  if(arg == "AGGRESSIVE_MODE_UNSUPPORTED") return AuthErrorCode::AGGRESSIVE_MODE_UNSUPPORTED;
  if(arg == "MAC_NOT_SUPPORTED") return AuthErrorCode::MAC_NOT_SUPPORTED;
  if(arg == "KEY_WRAP_NOT_SUPPORTED") return AuthErrorCode::KEY_WRAP_NOT_SUPPORTED;
  if(arg == "AUTHORIZATION_FAILED") return AuthErrorCode::AUTHORIZATION_FAILED;
  if(arg == "UPDATE_KEY_METHOD_NOT_PERMITTED") return AuthErrorCode::UPDATE_KEY_METHOD_NOT_PERMITTED;
  if(arg == "INVALID_SIGNATURE") return AuthErrorCode::INVALID_SIGNATURE;
  if(arg == "INVALID_CERTIFICATION_DATA") return AuthErrorCode::INVALID_CERTIFICATION_DATA;
  if(arg == "UNKNOWN_USER") return AuthErrorCode::UNKNOWN_USER;
  if(arg == "MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED") return AuthErrorCode::MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED;
  else return AuthErrorCode::UNKNOWN;
}


}
