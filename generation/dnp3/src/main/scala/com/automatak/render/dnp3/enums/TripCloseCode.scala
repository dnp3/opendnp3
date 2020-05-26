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


object TripCloseCode {

  private val comments = List(
    "Used in conjunction with Operation Type in a CROB to describe which output to operate for complementary two-output model",
    "Refer to section A.8.1 of IEEE 1815-2012 for a full description"
  )

  def apply(): EnumModel = EnumModel("TripCloseCode", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(
    EnumValue("NUL", 0x0, "Use the default output."),
    EnumValue("CLOSE", 0x1, "For complementary two-output model, operate the close output."),
    EnumValue("TRIP", 0x2, "For complementary two-output model, operate the trip output."),
    EnumValue("RESERVED", 0x3, "Reserved for future use.")
  )

}



