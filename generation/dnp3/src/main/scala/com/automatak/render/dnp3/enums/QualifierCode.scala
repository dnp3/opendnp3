package com.automatak.render.dnp3.enums

import com.automatak.render.{Hex, EnumModel, EnumValue}


object QualifierCode {

  def apply(): EnumModel = EnumModel("QualifierCode", Some(EnumModel.UInt8), codes, Some(codes.last), Hex)

  private val codes = List(
    EnumValue("1B_START_STOP", 0x00),
    EnumValue("2B_START_STOP", 0x01),
    EnumValue("4B_START_STOP", 0x02),
    EnumValue("ALL_OBJ", 0x06),
    EnumValue("1B_CNT", 0x07),
    EnumValue("2B_CNT", 0x08),
    EnumValue("4B_CNT", 0x09),
    EnumValue("1B_CNT_1B_INDEX", 0x17),
    EnumValue("2B_CNT_2B_INDEX", 0x28),
    EnumValue("4B_CNT_4B_INDEX", 0x39),
    EnumValue("UNDEFINED", 0xFF)
  )

}
