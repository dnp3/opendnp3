package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToAnalog, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

object Group30 extends ObjectGroup {
  def objects = List(Group30Var1, Group30Var2, Group30Var3, Group30Var4, Group30Var5, Group30Var6)
  def group: Byte = 30
}

object Group30Var1 extends FixedSize(Group30, 1)(flags, value32) with ConversionToAnalog
object Group30Var2 extends FixedSize(Group30, 2)(flags, value16) with ConversionToAnalog
object Group30Var3 extends FixedSize(Group30, 3)(value32) with ConversionToAnalog
object Group30Var4 extends FixedSize(Group30, 4)(value16) with ConversionToAnalog
object Group30Var5 extends FixedSize(Group30, 5)(flags, float32) with ConversionToAnalog
object Group30Var6 extends FixedSize(Group30, 6)(flags, float64) with ConversionToAnalog
