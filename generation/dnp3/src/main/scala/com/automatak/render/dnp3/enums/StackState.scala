package com.automatak.render.dnp3.enums

import com.automatak.render._

object StackState {

  private val comments = List("Enumeration for possible communication states of a stack")

  def apply(): EnumModel = EnumModel("StackState", comments, EnumModel.Integer, codes, codes.last, Base10)

  private val codes = List(
    EnumValue("COMMS_UP", 0, "communications are online"),
    EnumValue("COMMS_DOWN", 1, "communication difficulties"),
    EnumValue("UNKNOWN", 2, "unknown state")
  )

}



