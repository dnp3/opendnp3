package com.automatak.render.dnp3.enums

import com.automatak.render._


object CommandResult {

  private val comments = List("Opendnp3 API enum used for differentiating cases when a command sequence fails without a response from the outstation")

  def apply(): EnumModel = EnumModel("CommandResult", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
       EnumValue("RESPONSE_OK", 0, None, "A response was received from the outstation, check the CommandStatus enumeration"),
       EnumValue("BAD_RESPONSE", 1, None, "A response was received from the outstation, but it did not match or contained bad formatting"),
       EnumValue("TIMEOUT", 2, None,  "The operation timed out without a response"),
       EnumValue("NO_COMMS", 3, None,  "There is no communication with the outstation, and the command was not attempted"),
       EnumValue("QUEUE_FULL", 4, None, "The master's requst queue is full. Too many operations have been requested")
  )

}



