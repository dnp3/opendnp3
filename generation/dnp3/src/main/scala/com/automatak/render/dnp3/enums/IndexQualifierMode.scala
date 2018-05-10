package com.automatak.render.dnp3.enums

import com.automatak.render._


object IndexQualifierMode {

  private val comments = List("Specifies whether opendnp3 optimizes for 1-byte indexes when making requests")

  def apply(): EnumModel = EnumModel("IndexQualifierMode", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = List(
    EnumValue("allow_one_byte", 0, "Use a one byte qualifier if possible"),
    EnumValue("always_two_bytes", 1, "Always use two byte qualifiers even if the index is <= 255")
  )

}



