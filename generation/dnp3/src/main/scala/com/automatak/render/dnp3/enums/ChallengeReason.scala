package com.automatak.render.dnp3.enums

import com.automatak.render._


object ChallengeReason {

  private val comments = List(
    "Enumerates reasons for a sec-auth challenge"
  )

  def apply(): EnumModel = EnumModel("ChallengeReason", comments, EnumModel.UInt8, codes, Hex)

  private val codes = List(
    EnumValue("CRITICAL", 1, "Challenging a critical function"),
    EnumValue("UNKNOWN", 255, "Unknown reason")
  )

}



