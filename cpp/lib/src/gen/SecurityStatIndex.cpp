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

#include "opendnp3/gen/SecurityStatIndex.h"
#include <stdexcept>

namespace opendnp3 {

uint8_t SecurityStatIndexToType(SecurityStatIndex arg)
{
  return static_cast<uint8_t>(arg);
}
SecurityStatIndex SecurityStatIndexFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0):
      return SecurityStatIndex::UNEXPECTED_MESSAGES;
    case(1):
      return SecurityStatIndex::AUTHORIZATION_FAILURES;
    case(2):
      return SecurityStatIndex::AUTHENTICATION_FAILURES;
    case(3):
      return SecurityStatIndex::REPLY_TIMEOUTS;
    case(4):
      return SecurityStatIndex::REKEYS_DUE_TO_AUTH_FAILUE;
    case(5):
      return SecurityStatIndex::TOTAL_MESSAGES_TX;
    case(6):
      return SecurityStatIndex::TOTAL_MESSAGES_RX;
    case(7):
      return SecurityStatIndex::CRITICAL_MESSAGES_TX;
    case(8):
      return SecurityStatIndex::CRITICAL_MESSAGES_RX;
    case(9):
      return SecurityStatIndex::DISCARED_MESSAGES;
    case(10):
      return SecurityStatIndex::ERROR_MESSAGES_TX;
    case(11):
      return SecurityStatIndex::ERROR_MESSAGES_RX;
    case(12):
      return SecurityStatIndex::SUCCESSFUL_AUTHS;
    case(13):
      return SecurityStatIndex::SESSION_KEY_CHANGES;
    case(14):
      return SecurityStatIndex::FAILED_SESSION_KEY_CHANGES;
    case(15):
      return SecurityStatIndex::UPDATE_KEY_CHANGES;
    case(16):
      return SecurityStatIndex::FAILED_UPDATE_KEY_CHANGES;
    case(17):
      return SecurityStatIndex::REKEYS_DUE_TO_RESTART;
    default:
      throw new std::invalid_argument("Unknown value");
  }
}
char const* SecurityStatIndexToString(SecurityStatIndex arg)
{
  switch(arg)
  {
    case(SecurityStatIndex::UNEXPECTED_MESSAGES):
      return "UNEXPECTED_MESSAGES";
    case(SecurityStatIndex::AUTHORIZATION_FAILURES):
      return "AUTHORIZATION_FAILURES";
    case(SecurityStatIndex::AUTHENTICATION_FAILURES):
      return "AUTHENTICATION_FAILURES";
    case(SecurityStatIndex::REPLY_TIMEOUTS):
      return "REPLY_TIMEOUTS";
    case(SecurityStatIndex::REKEYS_DUE_TO_AUTH_FAILUE):
      return "REKEYS_DUE_TO_AUTH_FAILUE";
    case(SecurityStatIndex::TOTAL_MESSAGES_TX):
      return "TOTAL_MESSAGES_TX";
    case(SecurityStatIndex::TOTAL_MESSAGES_RX):
      return "TOTAL_MESSAGES_RX";
    case(SecurityStatIndex::CRITICAL_MESSAGES_TX):
      return "CRITICAL_MESSAGES_TX";
    case(SecurityStatIndex::CRITICAL_MESSAGES_RX):
      return "CRITICAL_MESSAGES_RX";
    case(SecurityStatIndex::DISCARED_MESSAGES):
      return "DISCARED_MESSAGES";
    case(SecurityStatIndex::ERROR_MESSAGES_TX):
      return "ERROR_MESSAGES_TX";
    case(SecurityStatIndex::ERROR_MESSAGES_RX):
      return "ERROR_MESSAGES_RX";
    case(SecurityStatIndex::SUCCESSFUL_AUTHS):
      return "SUCCESSFUL_AUTHS";
    case(SecurityStatIndex::SESSION_KEY_CHANGES):
      return "SESSION_KEY_CHANGES";
    case(SecurityStatIndex::FAILED_SESSION_KEY_CHANGES):
      return "FAILED_SESSION_KEY_CHANGES";
    case(SecurityStatIndex::UPDATE_KEY_CHANGES):
      return "UPDATE_KEY_CHANGES";
    case(SecurityStatIndex::FAILED_UPDATE_KEY_CHANGES):
      return "FAILED_UPDATE_KEY_CHANGES";
    case(SecurityStatIndex::REKEYS_DUE_TO_RESTART):
      return "REKEYS_DUE_TO_RESTART";
    default:
      return "UNDEFINED";
  }
}
SecurityStatIndex SecurityStatIndexFromString(const std::string& arg)
{
  if(arg == "UNEXPECTED_MESSAGES") return SecurityStatIndex::UNEXPECTED_MESSAGES;
  if(arg == "AUTHORIZATION_FAILURES") return SecurityStatIndex::AUTHORIZATION_FAILURES;
  if(arg == "AUTHENTICATION_FAILURES") return SecurityStatIndex::AUTHENTICATION_FAILURES;
  if(arg == "REPLY_TIMEOUTS") return SecurityStatIndex::REPLY_TIMEOUTS;
  if(arg == "REKEYS_DUE_TO_AUTH_FAILUE") return SecurityStatIndex::REKEYS_DUE_TO_AUTH_FAILUE;
  if(arg == "TOTAL_MESSAGES_TX") return SecurityStatIndex::TOTAL_MESSAGES_TX;
  if(arg == "TOTAL_MESSAGES_RX") return SecurityStatIndex::TOTAL_MESSAGES_RX;
  if(arg == "CRITICAL_MESSAGES_TX") return SecurityStatIndex::CRITICAL_MESSAGES_TX;
  if(arg == "CRITICAL_MESSAGES_RX") return SecurityStatIndex::CRITICAL_MESSAGES_RX;
  if(arg == "DISCARED_MESSAGES") return SecurityStatIndex::DISCARED_MESSAGES;
  if(arg == "ERROR_MESSAGES_TX") return SecurityStatIndex::ERROR_MESSAGES_TX;
  if(arg == "ERROR_MESSAGES_RX") return SecurityStatIndex::ERROR_MESSAGES_RX;
  if(arg == "SUCCESSFUL_AUTHS") return SecurityStatIndex::SUCCESSFUL_AUTHS;
  if(arg == "SESSION_KEY_CHANGES") return SecurityStatIndex::SESSION_KEY_CHANGES;
  if(arg == "FAILED_SESSION_KEY_CHANGES") return SecurityStatIndex::FAILED_SESSION_KEY_CHANGES;
  if(arg == "UPDATE_KEY_CHANGES") return SecurityStatIndex::UPDATE_KEY_CHANGES;
  if(arg == "FAILED_UPDATE_KEY_CHANGES") return SecurityStatIndex::FAILED_UPDATE_KEY_CHANGES;
  if(arg == "REKEYS_DUE_TO_RESTART") return SecurityStatIndex::REKEYS_DUE_TO_RESTART;
  else throw std::invalid_argument(std::string("Unknown value: ") + arg);
}

}
