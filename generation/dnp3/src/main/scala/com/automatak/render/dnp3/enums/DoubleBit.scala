/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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

object DoubleBit {

  private val comments = List("Enumeration for possible states of a double bit value")

  def apply(): EnumModel = EnumModel("DoubleBit", comments, EnumModel.UInt8, codes, Some(defaultValue), Hex)

  def defaultValue = EnumValue("INDETERMINATE", 3, "Abnormal or custom condition")

  private val codes = List(
    EnumValue("INTERMEDIATE", 0, "Transitioning between end conditions"),
    EnumValue("DETERMINED_OFF", 1, "End condition, determined to be OFF"),
    EnumValue("DETERMINED_ON", 2, "End condition, determined to be ON")
  )

}



