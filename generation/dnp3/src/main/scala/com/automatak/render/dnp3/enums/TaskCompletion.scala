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


object TaskCompletion {

  private val comments = List("Enum that describes if a master task succeeded or failed")

  private val defaultValue = EnumValue("FAILURE_NO_COMMS", 255, "There is no communication with the outstation, so the task was not attempted")

  def apply(): EnumModel = EnumModel("TaskCompletion", comments, EnumModel.UInt8, codes, Some(defaultValue), Base10)

  private val codes = List(
       EnumValue("SUCCESS", 0, "A valid response was received from the outstation"),
       EnumValue("FAILURE_BAD_RESPONSE", 1, "A response was received from the outstation, but it was not valid"),
       EnumValue("FAILURE_RESPONSE_TIMEOUT", 2, "The task request did not receive a response within the timeout"),
       EnumValue("FAILURE_START_TIMEOUT", 3, "The start timeout expired before the task could begin running"),
       EnumValue("FAILURE_MESSAGE_FORMAT_ERROR", 4, "The task failed because the master was unable to format the request")
  )

}



