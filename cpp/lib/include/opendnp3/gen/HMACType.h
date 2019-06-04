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

#ifndef OPENDNP3_HMACTYPE_H
#define OPENDNP3_HMACTYPE_H

#include <cstdint>

namespace opendnp3 {

/**
  Enumerates possible algorithms used to calculate the HMAC
*/
enum class HMACType : uint8_t
{
  /// No HMAC value in this message
  NO_MAC_VALUE = 0x0,
  /// Networked version of SHA1
  HMAC_SHA1_TRUNC_10 = 0x2,
  /// Serial version of SHA256
  HMAC_SHA256_TRUNC_8 = 0x3,
  /// Networked version of SHA256
  HMAC_SHA256_TRUNC_16 = 0x4,
  /// Serial version of SHA1
  HMAC_SHA1_TRUNC_8 = 0x5,
  /// 12 octet output for serial or network
  AES_GMAC = 0x6,
  /// Unknown HMAC algorithm
  UNKNOWN = 0xFF
};

uint8_t HMACTypeToType(HMACType arg);
HMACType HMACTypeFromType(uint8_t arg);
char const* HMACTypeToString(HMACType arg);

}

#endif
