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

import com.automatak.render.{EnumValue, Base10, EnumModel}

object GroupVariationType {

  private val comments = List("Enumeration for possible states of a channel")

  private val undefined = EnumValue("UNKNOWN", 5, "object type is undefined")

  def apply(): EnumModel = EnumModel("GroupVariationType", comments, EnumModel.UInt8, types, Some(undefined), Base10)

  private val types = List(
    EnumValue("FIXED_SIZE", 0, "fixed size type"),
    EnumValue("SINGLE_BITFIELD", 1, "single bit-field type"),
    EnumValue("DOUBLE_BITFIELD", 2, "double bit-field type"),
    EnumValue("SIZELESS", 3, "object has no size and is only used in requests"),
    EnumValue("SIZED_BY_VARIATION", 4, "the size of the object is the object's variation"),
    EnumValue("VARIABLE_SIZE", 5, "object size varies and requires free-format header")
  )

}