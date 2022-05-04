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


object TimeSyncMode {

  private val comments = List(
    "Determines what the master station does when it sees the NEED_TIME iin bit"
  )

  def defaultValue = EnumValue("None", 0, "don't perform a time-sync")

  def apply(): EnumModel = EnumModel("TimeSyncMode", comments, EnumModel.UInt8, codes, Some(defaultValue), Base10)

  private val codes = List(
    EnumValue("NonLAN", 1, "synchronize the outstation's time using the non-LAN time sync procedure"),
    EnumValue("LAN", 2, "synchronize the outstation's time using the LAN time sync procedure")
  )

}



