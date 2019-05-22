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

#ifndef OPENDNP3_KEYCHANGEMETHOD_H
#define OPENDNP3_KEYCHANGEMETHOD_H

#include <cstdint>

namespace opendnp3 {

/**
  Enumerates possible algorithms for changing the update key
*/
enum class KeyChangeMethod : uint8_t
{
  AES_128_SHA1_HMAC = 0x3,
  AES_256_SHA256_HMAC = 0x4,
  AES_256_AES_GMAC = 0x5,
  RSA_1024_DSA_SHA1_HMAC_SHA1 = 0x43,
  RSA_2048_DSA_SHA256_HMAC_SHA256 = 0x44,
  RSA_3072_DSA_SHA256_HMAC_SHA256 = 0x45,
  RSA_2048_DSA_SHA256_AES_GMAC = 0x46,
  RSA_3072_DSA_SHA256_AES_GMAC = 0x47,
  UNDEFINED = 0x0
};

uint8_t KeyChangeMethodToType(KeyChangeMethod arg);
KeyChangeMethod KeyChangeMethodFromType(uint8_t arg);
char const* KeyChangeMethodToString(KeyChangeMethod arg);

}

#endif
