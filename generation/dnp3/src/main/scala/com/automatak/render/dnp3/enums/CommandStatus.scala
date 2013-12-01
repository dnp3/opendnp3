package com.automatak.render.dnp3.enums

import com.automatak.render._


object CommandStatus {

  def apply(): EnumModel = EnumModel("CommandStatus", EnumModel.UInt8, codes, codes.last, Base10)

  private val codes = List(
    EnumValue("SUCCESS", 0, "command was successfully received and handled"),
    EnumValue("TIMEOUT", 1, "command timed out before completing"),
    EnumValue("NO_SELECT", 2, "command requires being selected before operate, configuration issue"),
    EnumValue("FORMAT_ERROR", 3, "bad control code or timing values"),
    EnumValue("NOT_SUPPORTED", 4, "command is not implemented"),
    EnumValue("ALREADY_ACTIVE", 5, "command is all ready in progress or its all ready in that mode"),
    EnumValue("HARDWARE_ERROR", 6, "something is stopping the command, often a local/remote interlock"),
    EnumValue("LOCAL", 7, "the function governed by the control is in local only control"),
    EnumValue("TOO_MANY_OPS", 8, "the command has been done too often and has been throttled"),
    EnumValue("NOT_AUTHORIZED", 9, "the command was rejected because the device denied it or an RTU intercepted it"),
    EnumValue("UNDEFINED", 127, "10 to 126 are currently reserved")
  )


}



