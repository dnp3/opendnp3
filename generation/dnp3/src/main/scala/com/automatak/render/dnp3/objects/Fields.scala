package com.automatak.render.dnp3.objects

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums.{ControlCode, CommandStatus}

sealed trait FieldType
class FixedSizeFieldType(val numBytes: Int) extends FieldType

case object UInt8Field extends FixedSizeFieldType(1)
case object UInt16Field extends FixedSizeFieldType(2)
case object UInt32Field extends FixedSizeFieldType(4)
case object UInt48Field extends FixedSizeFieldType(6)
case class EnumField(model: EnumModel) extends FixedSizeFieldType(1)


object FixedSizeField {

  //common flags field
  val flags = FixedSizeField("flags", UInt8Field)

  // timestamps
  val time16 = FixedSizeField("time16", UInt16Field)
  val time48 = FixedSizeField("time48", UInt48Field)

  // counter values
  val count16 = FixedSizeField("count", UInt16Field)
  val count32 = FixedSizeField("count", UInt32Field)


  //enums
  val controlCode = FixedSizeField("controlCode", EnumField(ControlCode()))
  val commandStatus = FixedSizeField("status", EnumField(CommandStatus()))
}


case class FixedSizeField(name: String, typ: FixedSizeFieldType)

