package com.automatak.render.dnp3.objects

sealed trait FieldType
class FixedSizeFieldType(val numBytes: Int) extends FieldType

case object UInt8Field extends FixedSizeFieldType(1)
case object UInt16Field extends FixedSizeFieldType(2)
case object UInt48Field extends FixedSizeFieldType(6)


object FixedSizeField {
  val flags = FixedSizeField("flags", UInt8Field)

  val time16 = FixedSizeField("time16", UInt16Field)
  val time48 = FixedSizeField("time48", UInt48Field)
}


case class FixedSizeField(name: String, typ: FixedSizeFieldType)

