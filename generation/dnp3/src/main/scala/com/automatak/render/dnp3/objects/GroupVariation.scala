/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

import com.automatak.render.Indentation
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects.generators._

object GroupVariation {
  case class Id(group: Byte, variation: Byte)
}


/**
 * Base trait for DNP3 objects
 */
trait GroupVariation {

  import GroupVariation.Id

  final def group: Byte = parent.group
  final def id: Id = Id(group, variation)
  final def name: String = "%sVar%s".format(parent.name, variation)
  final def fullDesc: String = "%s - %s".format(parent.desc, desc)
  final def shortValue : Int = group*256 + variation

  def variation: Byte
  def parent: ObjectGroup
  def desc: String
  def declaration(implicit i : Indentation): Iterator[String] = struct(name) { headerLines }

  def attributes : Set[FieldAttribute.Value]

  /// --- Includes for h/cpp files ----

  def headerIncludes : List[String] = List(""""opendnp3/app/GroupVariationID.h"""") // always included in headers
  def implIncludes : List[String] = List(""""app/parsing/DNPTimeParsing.h"""")

  /// --- actual source lines for the h/cpp ---

  def headerLines(implicit i : Indentation) : Iterator[String] = GroupVariationLines.idDeclaration(this)
  def implLines(implicit i : Indentation) : Iterator[String] = Iterator.empty
}

class AnyVariation(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v, "Any Variation")
class ClassData(g: ObjectGroup, v: Byte, desc: String) extends BasicGroupVariation(g,v, desc)
class SizedByVariation(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v, "Sized by variation")
abstract class DefaultVariableSize(g: ObjectGroup, v: Byte, description: String) extends BasicGroupVariation(g,v,description)
class SingleBitfield(g: ObjectGroup, v: Byte, description: String) extends BasicGroupVariation(g,v,description)
class DoubleBitfield(g: ObjectGroup, v: Byte, description: String) extends BasicGroupVariation(g,v,description)

sealed abstract class BasicGroupVariation(g: ObjectGroup, v: Byte, description: String) extends  GroupVariation {
  def variation: Byte = v
  def parent: ObjectGroup = g
  def desc: String = description

  def attributes : Set[FieldAttribute.Value] = Set.empty
}

class AuthVariableSize( g: ObjectGroup,
                        v: Byte,
                        description: String,
                        val fixedFields: List[FixedSizeField],
                        val lengthFields: List[VariableField],
                        val remainder: Option[VariableField]) extends BasicGroupVariation(g,v,description)  {

  override def headerIncludes: List[String] = super.headerIncludes ++ GroupVariationIncludes.headerReadWrite ++ GroupVariationIncludes.headerVariableLength ++ FixedSizeHelpers.fieldHeaders(fixedFields)
  override def implIncludes: List[String] = super.implIncludes ++ GroupVariationIncludes.implVariableLength ++ GroupVariationIncludes.implReadWrite

  override def declaration(implicit i : Indentation) : Iterator[String] = struct(name, Some("IVariableLength"))(headerLines)

  override def headerLines(implicit i : Indentation) : Iterator[String] = super.headerLines ++ AuthVariableSizeGenerator.header(this)
  override def implLines(implicit i : Indentation) : Iterator[String] = super.implLines ++ GroupVariationLines.implComments(this) ++ AuthVariableSizeGenerator.implementation(this)

  /// The total minimum size for the aggregate object
  def minimumSize : Int = {

    def fixedSize = fixedFields.map(x => x.typ.numBytes).sum
    def variableSize = 2*lengthFields.length

    fixedSize + variableSize
  }

}

class RemainderOnly( g: ObjectGroup,  v: Byte, description: String, remainder: VariableField) extends AuthVariableSize(g, v, description, Nil, Nil, Some(remainder)) {
  def remainderValue : VariableField = remainder
}

class FixedSize(g: ObjectGroup, v: Byte, description: String)(fs: FixedSizeField*) extends BasicGroupVariation(g,v, description) {

  override def headerIncludes: List[String] = super.headerIncludes ++ GroupVariationIncludes.headerReadWrite
  override def implIncludes: List[String] = super.implIncludes ++ GroupVariationIncludes.implReadWrite

  override def headerLines(implicit i : Indentation) : Iterator[String] = super.headerLines ++ FixedSizeGenerator.header(this)
  override def implLines(implicit i : Indentation) : Iterator[String] = super.implLines ++ GroupVariationLines.implComments(this) ++ FixedSizeGenerator.implementation(this)

  def fields : List[FixedSizeField] = fs.toList
  def size: Int = fs.map(x => x.typ.numBytes).sum

  override def attributes : Set[FieldAttribute.Value] = fs.flatMap(_.attributes).toSet
}
