package com.automatak.render.dnp3.enums

import com.automatak.render._

object ChannelState {

  private val comments = List("Enumeration for possible states of a channel")

  def apply(): EnumModel = EnumModel("ChannelState", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
    EnumValue("CLOSED", 0, None, "offline and idle"),
    EnumValue("OPENING", 1, None, "trying to open"),
    EnumValue("WAITING", 2, None, "waiting to open"),
    EnumValue("OPEN", 3, None, "open"),
    EnumValue("SHUTDOWN", 4, None, "stopped and will never do anything again")
  )

}



