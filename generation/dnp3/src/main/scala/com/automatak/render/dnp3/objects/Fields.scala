package com.automatak.render.dnp3.objects

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums._

sealed trait FieldType
sealed class FixedSizeFieldType(val numBytes: Int) extends FieldType {
  def defaultValue: String = "0"
}

case object UInt8Field extends FixedSizeFieldType(1)
case object UInt16Field extends FixedSizeFieldType(2)
case object UInt32Field extends FixedSizeFieldType(4)
case object UInt48Field extends FixedSizeFieldType(6)
case object SInt16Field extends FixedSizeFieldType(2)
case object SInt32Field extends FixedSizeFieldType(4)
case object Float32Field extends FixedSizeFieldType(4)
case object Float64Field extends FixedSizeFieldType(8)
case class EnumField(model: EnumModel) extends FixedSizeFieldType(1) {
  override def defaultValue: String = "%s::%s".format(model.name, model.default.displayName)
}

object FixedSizeField {

  //common flags field
  val flags = FixedSizeField("flags", UInt8Field)


  // SA stuff
  val csq = FixedSizeField("challengeSeqNum", UInt32Field)
  val ksq = FixedSizeField("keyChangeSeqNum", UInt32Field)
  val user = FixedSizeField("userNum", UInt16Field)
  val assocId = FixedSizeField("assocId", UInt16Field)
  val macAlgo = FixedSizeField("macAlgo", EnumField(HMACType()))
  val keyWrapAlgo = FixedSizeField("keyWrapAlgo", EnumField(KeyWrapAlgorithm()))
  val keyStatus = FixedSizeField("keyStatus", EnumField(KeyStatus()))
  val challengeReason = FixedSizeField("challengeReason", EnumField(ChallengeReason()))
  val errorCode = FixedSizeField("errorCode", EnumField(AuthErrorCode()))


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

object VariableFields {
  val challengeData = VariableField("challengeData")
  val hmac = VariableField("hmac")
  val keyWrapData = VariableField("keyWrapData")
  val errorText = VariableField("errorText")
  val certificate = VariableField("certificate")
}

case class FixedSizeField(name: String, typ: FixedSizeFieldType)

case class VariableField(name: String)


