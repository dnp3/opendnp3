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


  def variation: Byte
  def parent: ObjectGroup
}

class AnyVariation(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)
class SingleBitfield(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)
class DoubleBitfield(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)

sealed abstract class BasicGroupVariation(g: ObjectGroup, v: Byte) extends  GroupVariation {
  def variation: Byte = v
  def parent: ObjectGroup = g
}

trait Conversion {
  def target: String
  def signatures : Iterator[String]
  def impl(fields: FixedSize)(implicit indent: Indentation): Iterator[String]
  def includeHeaders: List[String]
  def implHeaders: List[String]
}

class FixedSize(g: ObjectGroup, v: Byte)(fs: FixedSizeField*) extends BasicGroupVariation(g,v) {

  def conversion: Option[Conversion] = None // overridable

  def size: Int = fields.foldLeft(0)((sum, f) => sum + f.typ.numBytes)

  final def fields : List[FixedSizeField] = fs.toList

}