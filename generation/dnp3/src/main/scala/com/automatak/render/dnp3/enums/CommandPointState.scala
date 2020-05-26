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


object CommandPointState {

  private val comments = List(
    "List the various states that an individual command object can be in after an SBO or direct operate request"
  )

  def apply(): EnumModel = EnumModel("CommandPointState", comments, EnumModel.UInt8, EnumValues.fromPairs(codes), None, Base10)

  private val codes = List(
    "INIT" -> "No corresponding response was ever received for this command point",
    "SELECT_SUCCESS" -> "A response to a select request was received and matched, but the operate did not complete",
    "SELECT_MISMATCH" -> "A response to a select operation did not contain the same value that was sent",
    "SELECT_FAIL" -> "A response to a select operation contained a command status other than success",
    "OPERATE_FAIL" -> "A response to an operate or direct operate did not match the request",
    "SUCCESS" -> "A matching response was received to the operate"
  )


}



