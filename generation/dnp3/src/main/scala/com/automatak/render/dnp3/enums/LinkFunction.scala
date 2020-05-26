/**
 * Copyright 2013-2020 Automatak, LLC
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


object LinkFunction {

  private val comments = List("Link layer function code enumeration")

  def apply(): EnumModel = EnumModel("LinkFunction", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("INVALID", 0xFF)

  val codes = List(
    EnumValue("PRI_RESET_LINK_STATES", 0x40),
    EnumValue("PRI_TEST_LINK_STATES", 0x42),
    EnumValue("PRI_CONFIRMED_USER_DATA", 0x43),
    EnumValue("PRI_UNCONFIRMED_USER_DATA", 0x44),
    EnumValue("PRI_REQUEST_LINK_STATUS", 0x49),
    EnumValue("SEC_ACK", 0x00),
    EnumValue("SEC_NACK", 0x01),
    EnumValue("SEC_LINK_STATUS", 0x0B),
    EnumValue("SEC_NOT_SUPPORTED", 0x0F)
  )

}



