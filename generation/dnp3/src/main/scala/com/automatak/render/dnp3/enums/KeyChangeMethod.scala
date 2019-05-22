/*
 * Copyright 2013-2019 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.render.dnp3.enums

import com.automatak.render._


object KeyChangeMethod {

  private val comments = List("Enumerates possible algorithms for changing the update key")
  private val defaultValue = EnumValue("UNDEFINED", 0)

  def apply(): EnumModel = EnumModel("KeyChangeMethod", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val codes = List(
    EnumValue("AES_128_SHA1_HMAC", 3),
    EnumValue("AES_256_SHA256_HMAC", 4),
    EnumValue("AES_256_AES_GMAC", 5),
    EnumValue("RSA_1024_DSA_SHA1_HMAC_SHA1", 67),
    EnumValue("RSA_2048_DSA_SHA256_HMAC_SHA256", 68),
    EnumValue("RSA_3072_DSA_SHA256_HMAC_SHA256", 69),
    EnumValue("RSA_2048_DSA_SHA256_AES_GMAC", 70),
    EnumValue("RSA_3072_DSA_SHA256_AES_GMAC", 71)
  )

}



