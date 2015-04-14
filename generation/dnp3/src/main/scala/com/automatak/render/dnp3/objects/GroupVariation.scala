package com.automatak.render.dnp3.objects

import com.automatak.render.Indentation

object GroupVariation {
  case class Id(group: Byte, variation: Byte)
}

/**
 * Base trait for DNP3 objects
 */
sealed trait GroupVariation {

  import GroupVariation.Id

  final def group: Byte = parent.group
  final def id: Id = Id(group, variation)
  final def name: String = List("Group",group,"Var",variation).mkString
  final def fullDesc: String = "%s - %s".format(parent.desc, desc)
  final def shortValue : Int = group*256 + variation

  def variation: Byte
  def parent: ObjectGroup
  def desc: String
  def isFixedSize: Boolean = false

  def headerIncludes : List[String] = List(""""opendnp3/app/GroupVariationID.h"""") // always included in headers

  def implIncludes : List[String] = Nil

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
}

trait Conversion {

  def target: String
  def signatures : Iterator[String]
  def impl(fields: FixedSize)(implicit indent: Indentation): Iterator[String]

}

abstract class AuthVariableSize(g: ObjectGroup, v: Byte, description: String, val fixedFields: List[FixedSizeField], val lengthFields: List[VariableField], val remainder: Option[VariableField]) extends BasicGroupVariation(g,v,description)
{
  /// The total minimum size for the aggregate object
  def minimumSize : Int = {

    val fixedSize = fixedFields.foldLeft(0)((sum, f) => sum + f.typ.numBytes)
    val variableSize = lengthFields.foldLeft(0)((sum, v) => 2 + v.minLength.getOrElse(0))
    val remainderSize = remainder.map(r => r.minLength.getOrElse(0)).getOrElse(0)

    fixedSize + variableSize + remainderSize
  }

}

class FixedSize(g: ObjectGroup, v: Byte, description: String)(fs: FixedSizeField*) extends BasicGroupVariation(g,v, description) {

  final override def isFixedSize : Boolean = true

  override def headerIncludes = super.headerIncludes ++ Iterator(
    "<openpal/container/ReadBufferView.h>",
    "<openpal/container/WriteBufferView.h>",
    """"opendnp3/Types.h""""
  )

  override def implIncludes = super.headerIncludes ++ Iterator(
    "<openpal/serialization/Format.h>", "<openpal/serialization/Parse.h>"
  )

  def conversion: Option[Conversion] = None

  def size: Int = fields.foldLeft(0)((sum, f) => sum + f.typ.numBytes)

  final def fields : List[FixedSizeField] = fs.toList

}