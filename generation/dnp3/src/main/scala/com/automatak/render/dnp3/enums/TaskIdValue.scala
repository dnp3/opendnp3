package com.automatak.render.dnp3.enums

import com.automatak.render._


object TaskIdValue {

  private val comments = List("Enumeration of integer values for task ids")

  def apply(): EnumModel = EnumModel("TaskIdValue", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
      EnumValue("CLEAR_RESTART", 0),
      EnumValue("DISABLE_UNSOLICITED", 1),
      EnumValue("ASSIGN_CLASS", 2),
      EnumValue("STARTUP_INTEGRITY_POLL", 3),
      EnumValue("SERIAL_TIME_SYNC", 4),
      EnumValue("ENABLE_UNSOLICITED", 5),
      EnumValue("AUTO_EVENT_SCAN", 6)
  )

}



