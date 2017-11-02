package com.automatak.render.dnp3.enums

import com.automatak.render._


object ServerAcceptMode {

  private val comments = List(
    "Describes how TCP/TLS server channels handle new connections when an existing connection is already active"
  )

  def apply(): EnumModel = EnumModel("ServerAcceptMode", comments, EnumModel.UInt8, codes, None, Base10)

  private val codes = EnumValues.from(
    List(
    "CloseNew",
    "CloseExisting"
    )
  )

}



