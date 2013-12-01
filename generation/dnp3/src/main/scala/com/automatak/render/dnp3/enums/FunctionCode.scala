package com.automatak.render.dnp3.enums

import com.automatak.render._


object FunctionCode {

  private val comments = List("Application layer function code enumeration")

  def apply(): EnumModel = EnumModel("FunctionCode", comments, EnumModel.UInt8, codes, codes.last, Hex)

  private val codes = List(
    EnumValue("CONFIRM",0),
    EnumValue("READ",1),
    EnumValue("WRITE",2),
    EnumValue("SELECT",3),
    EnumValue("OPERATE",4),
    EnumValue("DIRECT_OPERATE",5),
    EnumValue("DIRECT_OPERATE_NO_ACK", 6),
    EnumValue("FREEZE",7),
    EnumValue("FREEZE_NO_ACK",8),
    EnumValue("FREEZE_CLEAR",9),
    EnumValue("FREEZE_CLEAR_NO_ACK",10),
    EnumValue("FREEZE_AT_TIME",11),
    EnumValue("FREEZE_AT_TIME_NO_ACK",12),
    EnumValue("COLD_RESTART", 13),
    EnumValue("WARM_RESTART", 14),
    EnumValue("INITIALIZE_DATA", 15),
    EnumValue("INITIALIZE_APPLICATION", 16),
    EnumValue("START_APPLICATION", 17),
    EnumValue("STOP_APPLICATION", 18),
    EnumValue("SAVE_CONFIGURATION", 19),
    EnumValue("ENABLE_UNSOLICITED", 20),
    EnumValue("DISABLE_UNSOLICITED", 21),
    EnumValue("ASSIGN_CLASS", 22),
    EnumValue("DELAY_MEASURE", 23),
    EnumValue("RECORD_TIME",24),
    EnumValue("FILE_OPEN", 25),
    EnumValue("FILE_CLOSE", 26),
    EnumValue("FILE_DELETE", 27),
    EnumValue("FILE_INFO", 28),
    EnumValue("FILE_AUTHENTICATE", 29),
    EnumValue("FILE_ABORT", 30),
    EnumValue("RESPONSE", 129),
    EnumValue("UNSOLICITED_RESPONSE", 130),
    EnumValue("UNKNOWN", 255))
}



