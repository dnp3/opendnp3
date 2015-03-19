package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToBinaryOutputStatus, FixedSize, ObjectGroup, FixedSizeField}
import com.automatak.render.dnp3.objects.VariationNames._

import FixedSizeField._

object Group11 extends ObjectGroup {
  def objects = List(Group11Var1, Group11Var2)
  def group: Byte = 11
  def desc: String = "Binary Output Event"
}

object Group11Var1 extends FixedSize(Group11, 1, outputStatusWithoutTime)(flags) with ConversionToBinaryOutputStatus
object Group11Var2 extends FixedSize(Group11, 2, outputStatusWithTime)(flags, time48) with ConversionToBinaryOutputStatus
