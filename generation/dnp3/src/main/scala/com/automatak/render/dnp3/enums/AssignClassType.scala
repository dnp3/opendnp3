package com.automatak.render.dnp3.enums

import com.automatak.render._


object AssignClassType {

  private val comments = List(
    "groups that can be used inconjunction with the ASSIGN_CLASS function code"
  )

  def apply(): EnumModel = EnumModel("AssignClassType", comments, EnumModel.UInt8, codes, Hex)

  private val codes = List(
    EnumValue("BinaryInput", 0),
    EnumValue("DoubleBinaryInput", 1),
    EnumValue("Counter", 2),
    EnumValue("FrozenCounter", 3),
    EnumValue("AnalogInput", 4),
    EnumValue("BinaryOutputStatus", 5),
    EnumValue("AnalogOutputStatus", 6)
  )

}



