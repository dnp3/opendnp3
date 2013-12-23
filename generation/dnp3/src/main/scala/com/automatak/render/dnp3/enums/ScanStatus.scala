package com.automatak.render.dnp3.enums

import com.automatak.render._


object ScanStatus {

  private val comments = List("Enumeration for the asynchronous result of a scan (poll)")

  def apply(): EnumModel = EnumModel("ScanStatus", comments, EnumModel.Integer, codes, Base10)

  private val codes = List(
       EnumValue("SUCCESS", 0,  "Valid response was received"),
       EnumValue("FAILURE", 1,  "The operation timed out or explicitly failed")
  )

}



