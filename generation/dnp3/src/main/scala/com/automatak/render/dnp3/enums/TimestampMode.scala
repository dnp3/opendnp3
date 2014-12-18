package com.automatak.render.dnp3.enums

import com.automatak.render._

object TimestampMode {

  private val comments = List("Indicates the validity of a timestamp values for an entire object header")

  def apply(): EnumModel = EnumModel("TimestampMode", comments, EnumModel.Integer, codes, None, Base10)

  private val codes = List(
    EnumValue("INVALID", 0, "Timestamp is not valid, ignore the value and use a local timestamp"),
    EnumValue("SYNCHRONIZED", 1, "The timestamp is UTC synchronzied at the remote device"),
    EnumValue("UNSYNCHRONIZED", 2, "The device indicate the timstamp may be unsynchronized")
  )

}



