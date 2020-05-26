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

import com.automatak.render.{Hex, EnumModel, EnumValue}


object QualifierCode {

  private val comments = List("Object header range/prefix as a single enumeration")
  private val defaultValue = EnumValue("UNDEFINED", 0xFF, None, Some("unknown"))

  def apply(): EnumModel = EnumModel("QualifierCode", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val codes = List(
    EnumValue("UINT8_START_STOP", 0x00, None, Some("8-bit start stop")),
    EnumValue("UINT16_START_STOP", 0x01, None, Some("16-bit start stop")),
    EnumValue("ALL_OBJECTS", 0x06, None, Some("all objects")),
    EnumValue("UINT8_CNT", 0x07, None, Some("8-bit count")),
    EnumValue("UINT16_CNT", 0x08, None, Some("16-bit count")),
    EnumValue("UINT8_CNT_UINT8_INDEX", 0x17, None, Some("8-bit count and prefix")),
    EnumValue("UINT16_CNT_UINT16_INDEX", 0x28, None, Some("16-bit count and prefix"))
  )

}
