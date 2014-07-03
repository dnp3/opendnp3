package com.automatak.render.dnp3.enums

import com.automatak.render._

object DoubleBit {

  private val comments = List("Enumeration for possible states of a double bit value")

  def apply(): EnumModel = EnumModel("DoubleBit", comments, EnumModel.UInt8, codes, Hex)

  private val codes = List(
    EnumValue("INTERMEDIATE", 0, None, "Transitioning between end conditions"),
    EnumValue("DETERMINED_OFF", 1, None, "End condition, determined to be OFF"),
    EnumValue("DETERMINED_ON", 2, None, "End condition, determined to be ON"),
    EnumValue("INDETERMINATE", 3, None, "Abnormal or custom condition")
  )

}



