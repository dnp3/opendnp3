package com.automatak.render.dnp3.enums

import com.automatak.render._


object UserTaskResult {

  private val comments = List("Opendnp3 API enum used that describes the result of a user task")

  def apply(): EnumModel = EnumModel("UserTaskResult", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
       EnumValue("RESPONSE_OK", 0, "A valid response was received from the outstation"),
       EnumValue("BAD_RESPONSE", 1, "A response was received from the outstation, but it contained unexpected contents or was malformed"),
       EnumValue("TIMEOUT", 2, "The operation timed out without a response"),
       EnumValue("NO_COMMS", 3, "There is no communication with the outstation, and the command was not attempted")
  )

}



