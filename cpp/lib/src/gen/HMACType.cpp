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

#include "opendnp3/gen/HMACType.h"

namespace opendnp3 {

uint8_t HMACTypeToType(HMACType arg)
{
  return static_cast<uint8_t>(arg);
}
HMACType HMACTypeFromType(uint8_t arg)
{
  switch(arg)
  {
    case(0x0):
      return HMACType::NO_MAC_VALUE;
    case(0x2):
      return HMACType::HMAC_SHA1_TRUNC_10;
    case(0x3):
      return HMACType::HMAC_SHA256_TRUNC_8;
    case(0x4):
      return HMACType::HMAC_SHA256_TRUNC_16;
    case(0x5):
      return HMACType::HMAC_SHA1_TRUNC_8;
    case(0x6):
      return HMACType::AES_GMAC;
    default:
      return HMACType::UNKNOWN;
  }
}
char const* HMACTypeToString(HMACType arg)
{
  switch(arg)
  {
    case(HMACType::NO_MAC_VALUE):
      return "NO_MAC_VALUE";
    case(HMACType::HMAC_SHA1_TRUNC_10):
      return "HMAC_SHA1_TRUNC_10";
    case(HMACType::HMAC_SHA256_TRUNC_8):
      return "HMAC_SHA256_TRUNC_8";
    case(HMACType::HMAC_SHA256_TRUNC_16):
      return "HMAC_SHA256_TRUNC_16";
    case(HMACType::HMAC_SHA1_TRUNC_8):
      return "HMAC_SHA1_TRUNC_8";
    case(HMACType::AES_GMAC):
      return "AES_GMAC";
    default:
      return "UNKNOWN";
  }
}

}
