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



