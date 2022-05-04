/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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


object Parity {

  private val comments = List("Enumeration for setting serial port parity")

  def apply(): EnumModel = EnumModel("Parity", comments, EnumModel.UInt8, codes, Some(default), Base10)

  def default = EnumValue("None", 0)

  private val codes = List(
    EnumValue("Even", 1),
    EnumValue("Odd", 2)
  )

}

object FlowControl {

  private val comments = List("Enumeration for setting serial port flow control")

  def apply(): EnumModel = EnumModel("FlowControl", comments, EnumModel.UInt8, codes, Some(default), Base10)

  def default = EnumValue("None", 0)

  private val codes = List(
    EnumValue("Hardware", 1),
    EnumValue("XONXOFF", 2)
  )

}


object StopBits {

  private val comments = List("Enumeration for setting serial port stop bits")

  def apply(): EnumModel = EnumModel("StopBits", comments, EnumModel.UInt8, codes, Some(default), Base10)

  def default = EnumValue("None", 0)

  private val codes = List(
    EnumValue("One", 1),
    EnumValue("OnePointFive", 2),
    EnumValue("Two", 3)
  )

}
