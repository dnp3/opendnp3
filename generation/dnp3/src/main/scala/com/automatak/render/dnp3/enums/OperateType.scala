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


object OperateType {

  private val comments = List(
    "Various ways that an outstation can receive a request to operate a BO or AO point"
  )

  def apply(): EnumModel = EnumModel("OperateType", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(

    EnumValue("SelectBeforeOperate", 0x00, "The outstation received a valid prior SELECT followed by OPERATE"),
    EnumValue("DirectOperate",0x01, "The outstation received a direct operate request"),
    EnumValue("DirectOperateNoAck", 0x02, "The outstation received a direct operate no ack request")

  )

}



