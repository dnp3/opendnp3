package com.automatak.render.dnp3.enums

import com.automatak.render._


object StaticTypeBitmask {

  private val comments = List(
    "Bitmask values for all the static types"
  )

  def apply(): EnumModel = EnumModel("StaticTypeBitmask", comments, EnumModel.UInt16, codes, Hex)

  private val codes = List(
    EnumValue("BinaryInput", 0x01),
    EnumValue("DoubleBinaryInput", 0x02),
    EnumValue("Counter", 0x04),
    EnumValue("FrozenCounter", 0x08),
    EnumValue("AnalogInput", 0x10),
    EnumValue("BinaryOutputStatus", 0x20),
    EnumValue("AnalogOutputStatus", 0x40),
    EnumValue("TimeAndInterval", 0x80)
  )

}



