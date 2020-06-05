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


object EventMode {

  private val comments = List(
    "Describes how a transaction behaves with respect to event generation"
  )

  def apply(): EnumModel = EnumModel("EventMode", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(
    EnumValue("Detect", 0, "Detect events using the specific mechanism for that type"),
    EnumValue("Force", 1, "Force the creation of an event bypassing detection mechanism"),
    EnumValue("Suppress", 2, "Never produce an event regardless of changes"),
    EnumValue("EventOnly", 3, "Force the creation of an event bypassing detection mechanism, but does not update the static value")
  )

}
