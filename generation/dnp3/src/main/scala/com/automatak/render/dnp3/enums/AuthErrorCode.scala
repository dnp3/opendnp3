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


object AuthErrorCode {

  private val comments = List(
    "Specifies the reason that an auth error message was transmitted"
  )

  def apply(): EnumModel = EnumModel("AuthErrorCode", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("UNKNOWN", 0, "Unknown error code")

  private val codes = List(
    EnumValue("AUTHENTICATION_FAILED", 1, "Supplied auth information "),
    EnumValue("AGGRESSIVE_MODE_UNSUPPORTED", 4, "Aggressive mode not supported on this link"),
    EnumValue("MAC_NOT_SUPPORTED", 5, "The specified MAC algorithm is not supported"),
    EnumValue("KEY_WRAP_NOT_SUPPORTED", 6, "The key-wrap algorithm is not supported"),
    EnumValue("AUTHORIZATION_FAILED", 7, "Authentication passed, but that user not authorized for requested operation"),
    EnumValue("UPDATE_KEY_METHOD_NOT_PERMITTED", 8, "The outstation does not support specified key change method on this link"),
    EnumValue("INVALID_SIGNATURE", 9, "The digital signature supplied in a user status change object was invalid"),
    EnumValue("INVALID_CERTIFICATION_DATA", 10, "The certification data supplied in a user status change object was invalid"),
    EnumValue("UNKNOWN_USER", 11, "The master attempted to change the Update Key of a user without a valid prior user status change"),
    EnumValue("MAX_SESSION_KEY_STATUS_REQUESTS_EXCEEDED", 12, "The master on a different association has requred session key status too often")
  )

}



