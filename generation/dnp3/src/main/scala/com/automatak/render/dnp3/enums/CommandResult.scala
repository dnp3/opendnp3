package com.automatak.render.dnp3.enums

import com.automatak.render._


object CommandResult {

  def apply(): EnumModel = EnumModel("CommandResult", EnumModel.UInt8, codes, codes.last, Base10)

  private val codes = List(
       EnumValue("RESPONSE_OK", 0,  "A response was received from the outstation, check the CommandStatus enumeration"),
       EnumValue("TIMEOUT", 1,  "The operation timed out without a response"),
       EnumValue("NO_COMMS", 2,  "There is no communication with the outstation, and the command was not attempted")
  )

}



