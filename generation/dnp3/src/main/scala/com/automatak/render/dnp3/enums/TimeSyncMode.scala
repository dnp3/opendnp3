package com.automatak.render.dnp3.enums

import com.automatak.render._


object TimeSyncMode {

  private val comments = List(
    "Determines what the master station does when it sees the NEED_TIME iin bit"
  )

  def defaultValue = EnumValue("None", 0, "don't perform a time-sync")

  def apply(): EnumModel = EnumModel("TimeSyncMode", comments, EnumModel.UInt8, codes, Some(defaultValue), Base10)

  private val codes = List(
    EnumValue("NonLAN", 1, "synchronize the outstation's time using the non-LAN time sync procedure"),
    EnumValue("LAN", 2, "synchronize the outstation's time using the LAN time sync procedure")
  )

}



