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


object OperationType {

  private val comments = List(
    "Used in conjunction with Trip Close Code in a CROB to describe what action to perform",
    "Refer to section A.8.1 of IEEE 1815-2012 for a full description"
  )

  def apply(): EnumModel = EnumModel("OperationType", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  private val defaultValue = EnumValue("Undefined", 0xFF, "Undefined.")

  private val codes = List(
    EnumValue("NUL", 0x0, "Do nothing."),
    EnumValue("PULSE_ON", 0x1, "Set output to active for the duration of the On-time."),
    EnumValue("PULSE_OFF", 0x2, "Non-interoperable code. Do not use for new applications."),
    EnumValue("LATCH_ON", 0x3, "Set output to active."),
    EnumValue("LATCH_OFF", 0x4, "Set the output to inactive.")
  )

}



