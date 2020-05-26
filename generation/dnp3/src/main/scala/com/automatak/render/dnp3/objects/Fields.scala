/**
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.render.dnp3.objects

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums._
import com.automatak.render.dnp3.objects.generators.FixedSizeHelpers

sealed abstract class FixedSizeFieldType(val numBytes: Int) {
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
case class EnumFieldType(model: EnumModel) extends FixedSizeFieldType(1) {
  override def defaultValue: String = model.default match {
    case Some(value) => "%s::%s".format(model.name, value.displayName)
    case None => throw new Exception(s"No default value for ${model.name}!")
  }
}

object FieldAttribute extends Enumeration {
  type WeekDay = Value
  val IsTimeUTC, IsTimeRel, IsFlags = Value
}

object FixedSizeField {

  //common flags field
  val flags = FixedSizeField("flags", UInt8Field, Set(FieldAttribute.IsFlags))

  // timestamps
  val time16 = FixedSizeField("time", UInt16Field, Set(FieldAttribute.IsTimeRel))
  val time48 = FixedSizeField("time", UInt48Field, Set(FieldAttribute.IsTimeUTC))

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
  val intervalUnit = FixedSizeField("intervalUnit", EnumFieldType(IntervalUnit()))

}

object VariableFields {
  val challengeData = VariableField("challengeData")
  val hmac = VariableField("hmacValue")
  val keyWrapData = VariableField("keyWrapData")
  val errorText = VariableField("errorText")
  val certificate = VariableField("certificate")
  val userName = VariableField("userName")
  val userPublicKey = VariableField("userPublicKey")
  val certificationData = VariableField("certificationData")
  val encryptedUpdateKey = VariableField("encryptedUpdateKey")
  val signature = VariableField("digitalSignature")
}

sealed trait Field {
  def name: String
  def cppType: String
  def cppArgument: String = cppType
}

sealed case class FixedSizeField(name: String, typ: FixedSizeFieldType, attributes: Set[FieldAttribute.Value] = Set.empty) extends Field {

  def isTimeUTC : Boolean = attributes.contains(FieldAttribute.IsTimeUTC)
  def isFlags : Boolean = attributes.contains(FieldAttribute.IsFlags)

  def cppType: String = typ match {
    case UInt8Field => "uint8_t"
    case UInt16Field => "uint16_t"
    case UInt32Field => "uint32_t"
    case UInt48Field => "DNPTime"
    case SInt16Field => "int16_t"
    case SInt32Field => "int32_t"
    case Float32Field => "float"
    case Float64Field => "double"
    case EnumFieldType(model: EnumModel) => model.name
    case _ => throw new Exception("Unknown field type")
  }

  def defaultValue: String = typ match {
    case UInt8Field => "0"
    case UInt16Field => "0"
    case UInt32Field => "0"
    case UInt48Field => "0"
    case SInt16Field => "0"
    case SInt32Field => "0"
    case Float32Field => "0.0"
    case Float64Field => "0.0"
    case EnumFieldType(model: EnumModel) => model.defaultValue.get.toString
    case _ => throw new Exception("Unknown field type")
  }

}
sealed case class VariableField(name: String) extends Field {
  def cppType: String = "ser4cpp::rseq_t"
  override def cppArgument = "const ser4cpp::rseq_t&"
}


