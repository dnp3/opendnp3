package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._

object Group2 extends ObjectGroup {
  val objects = List(Group2Var0, Group2Var1, Group2Var2, Group2Var3)
  def group: Byte = 2
}

object Group2Var0 extends AnyVariation(Group2, 0)
object Group2Var1 extends FixedSize(Group2, 1)(flags)
object Group2Var2 extends FixedSize(Group2, 2)(flags, time48)
object Group2Var3 extends FixedSize(Group2, 3)(flags, time16)