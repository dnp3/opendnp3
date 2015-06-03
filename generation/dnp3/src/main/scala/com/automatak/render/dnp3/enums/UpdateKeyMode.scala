package com.automatak.render.dnp3.enums

import com.automatak.render._


object UpdateKeyMode {

  private val comments = List("Enumerates types of update keys")


  def apply(): EnumModel = EnumModel("UpdateKeyMode", comments, EnumModel.UInt8, codes, None, Hex)

  private val codes = EnumValues.from(List("AES128", "AES256"))


}



