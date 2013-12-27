package com.automatak.render.dnp3.enums

import com.automatak.render.{Hex, EnumModel, EnumValue}


object QualifierCode {

  private val comments = List("Object header range/prefix as a single enumeration")

  def apply(): EnumModel = EnumModel("QualifierCode", comments, EnumModel.UInt8, codes, Hex)

  private val codes = List(
    EnumValue("UINT8_START_STOP", 0x00),
    EnumValue("UINT16_START_STOP", 0x01),
    EnumValue("ALL_OBJECTS", 0x06),
    EnumValue("UINT8_CNT", 0x07),
    EnumValue("UINT16_CNT", 0x08),
    EnumValue("UINT8_CNT_UINT8_INDEX", 0x17),
    EnumValue("UINT16_CNT_UINT16_INDEX", 0x28),
    EnumValue("UNDEFINED", 0xFF)
  )

}
