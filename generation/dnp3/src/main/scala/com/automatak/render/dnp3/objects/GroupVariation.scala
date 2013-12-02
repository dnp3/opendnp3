package com.automatak.render.dnp3.objects

/**
 * Base trait for DNP3 objects
 */
sealed trait GroupVariation {

  case class Id(group: Byte, variation: Byte)

  final def group: Byte = parent.group
  final def id: Id = Id(group, variation)

  def variation: Byte
  def parent: ObjectGroup
}

class AnyVariation(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)
class SingleBitfield(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)
class DoubleBitfield(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v)

class BasicGroupVariation(g: ObjectGroup, v: Byte) extends  GroupVariation {
  def variation: Byte = v
  def parent: ObjectGroup = g
}

abstract class FixedSizeGroupVariation(g: ObjectGroup, v: Byte) extends BasicGroupVariation(g,v) {

  def size: Int = fields.foldLeft(0)((sum, f) => sum + f.typ.numBytes)

  def fields : List[FixedSizeField]

}