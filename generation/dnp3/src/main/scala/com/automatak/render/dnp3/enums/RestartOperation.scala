package com.automatak.render.dnp3.enums

import com.automatak.render._

object RestartOperation {

  private val comments = List("Enumeration describing restart operation to perform on an outstation")

  def apply(): EnumModel = EnumModel("RestartOperation", comments, EnumModel.UInt8, codes, None, Base10)

  private val codes = List(
    EnumValue("COLD_RESTART", 0, "Full reboot"),
    EnumValue("WARM_RESTART", 1, "Warm reboot of process only")
  )

}



