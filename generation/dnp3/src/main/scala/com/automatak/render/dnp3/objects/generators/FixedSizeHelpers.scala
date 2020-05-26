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
package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.EnumModel
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects._

object FixedSizeHelpers {

  def getCppFieldTypeParser(x: FixedSizeFieldType): String = x match {
    case UInt8Field => "UInt8"
    case UInt16Field => "UInt16"
    case UInt32Field => "UInt32"
    case UInt48Field => "UInt48"
    case SInt16Field => "Int16"
    case SInt32Field => "Int32"
    case Float32Field => "SingleFloat"
    case Float64Field => "DoubleFloat"
    case EnumFieldType(model: EnumModel) => model.name
  }

  // Any special headers required for fixed-size fields
  def fieldHeaders(fields: List[FixedSizeField]): List[String] = fields.map { f =>
     f.typ match {
       case ef : EnumFieldType => List(quoted("gen/%sSerialization.h".format(ef.model.name)))
       case _ => Nil
     }
  }.flatten

  def fixedReadsOLD(fixedFields: List[FixedSizeField], returnBool: Boolean, bufferName: String, outputLocation: String) : Iterator[String] = {

    val returnStatement = returnBool match {
      case true => "result &= "
      case false => ""
    }

    def toNumericReadOp(fs: FixedSizeField) : Iterator[String] = {
      val tempVarName = "%sTemp".format(fs.name)

      if(fs.typ == UInt48Field) {
        Iterator(
          "UInt48Type %s;".format(tempVarName),
          returnStatement + "%s::read_from(%s, %s);".format(FixedSizeHelpers.getCppFieldTypeParser(fs.typ), bufferName, tempVarName),
          "%s%s = DNPTime(%s.Get());".format(outputLocation, fs.name, tempVarName)
        )
      }
      else {
        Iterator(returnStatement + "%s::read_from(%s, %s%s);".format(FixedSizeHelpers.getCppFieldTypeParser(fs.typ), bufferName, outputLocation, fs.name))
      }
    }

    def toEnumReadOp(fs: FixedSizeField, e: EnumFieldType) : Iterator[String] = {
      val rawValueName = fs.name ++ "RawValue"
      Iterator(
        "uint8_t %s;".format(rawValueName),
        returnStatement + "UInt8::read_from(%s, %s);".format(bufferName, rawValueName),
        "%s%s = %sFromType(%s);".format(outputLocation, fs.name, e.model.name, rawValueName)
      )
    }


    def toReadOp(fs: FixedSizeField) : Iterator[String] = fs.typ match {
      case x : EnumFieldType => toEnumReadOp(fs, x)
      case _ => toNumericReadOp(fs)
    }

    var lines = List[String]()
    if(returnBool) {
      lines ++= Iterator("bool result = true;") ++ space
    }
    if(fixedFields.nonEmpty) {
      lines ++= fixedFields.flatMap(toReadOp).iterator ++ space
    }
    if(returnBool) {
      lines ++= Iterator("return result;")
    }

    lines.iterator
  }

  def fixedReads(fixedFields: List[FixedSizeField], returnBool: Boolean, bufferName: String, inputLocation: String) : Iterator[String] = {

    def fieldParams() : String = {
      fixedFields.map(fs => "%s%s".format(inputLocation, fs.name)).mkString(", ")
    }

    if(fixedFields.isEmpty) {
      Iterator.empty
    }
    else {
      val returnStatement = returnBool match {
        case true => "return "
        case false => ""
      }
      Iterator(returnStatement + "LittleEndian::read(%s, %s);".format(bufferName, fieldParams))
    }
  }

  def fixedWrites(fixedFields: List[FixedSizeField], returnBool: Boolean, bufferName: String, inputLocation: String) : Iterator[String] = {

    def fieldParams() : String = {
      fixedFields.map(fs => "%s%s".format(inputLocation, fs.name)).mkString(", ")
    }

    if(fixedFields.isEmpty) {
      Iterator.empty
    }
    else {
      val returnStatement = returnBool match {
        case true => "return "
        case false => ""
      }
      Iterator(returnStatement + "LittleEndian::write(%s, %s);".format(bufferName, fieldParams))
    }
  }
}
