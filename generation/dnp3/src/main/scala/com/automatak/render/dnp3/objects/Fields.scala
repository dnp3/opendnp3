package com.automatak.render.dnp3.objects

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums.{IntervalUnit, ControlCode, CommandStatus}

sealed trait FieldType
class FixedSizeFieldType(val numBytes: Int) extends FieldType

case object UInt8Field extends FixedSizeFieldType(1)
case object UInt16Field extends FixedSizeFieldType(2)
case object UInt32Field extends FixedSizeFieldType(4)
case object UInt48Field extends FixedSizeFieldType(6)
case object SInt16Field extends FixedSizeFieldType(2)
case object SInt32Field extends FixedSizeFieldType(4)
case object Float32Field extends FixedSizeFieldType(4)
case object Float64Field extends FixedSizeFieldType(8)
case class EnumField(model: EnumModel) extends FixedSizeFieldType(1)

object FixedSizeField {

  //common flags field
  val flags = FixedSizeField("flags", UInt8Field)


  // timestamps
  val time16 = FixedSizeField("time", UInt16Field)
  val time48 = FixedSizeField("time", UInt48Field)

  // counter values
  val count16 = FixedSizeField("value", UInt16Field)
  val count32 = FixedSizeField("value", UInt32Field)

  // analog values
  val value16 = FixedSizeField("value", SInt16Field)
  val value32 = FixedSizeField("value", SInt32Field)
  val float32 = FixedSizeField("value", Float32Field)
  val float64 = FixedSizeField("value", Float64Field)

  //enums
  val commandStatus = FixedSizeField("status", UInt8Field)
  val intervalUnit = FixedSizeField("intervalUnit", EnumField(IntervalUnit()))

}


case class FixedSizeField(name: String, typ: FixedSizeFieldType)

