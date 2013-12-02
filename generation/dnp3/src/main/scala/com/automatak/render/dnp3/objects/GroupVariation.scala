package com.automatak.render.dnp3.objects

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

class FixedSize(g: ObjectGroup, v: Byte)(fs: FixedSizeField*) extends BasicGroupVariation(g,v) {

  def size: Int = fields.foldLeft(0)((sum, f) => sum + f.typ.numBytes)

  final def fields : List[FixedSizeField] = fs.toList

}