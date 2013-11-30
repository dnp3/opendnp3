package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

object Group1 extends ObjectGroup {
  def objects = List(Group1Var0, Group1Var2, Group1Var1)
  def group: Byte = 1
}

object Group1Var0 extends AnyVariation(Group1, 0)

object Group1Var1 extends SingleBitfield(Group1, 1)

object Group1Var2 extends FixedSizeGroupVariation(Group1, 2) {
  def fields = List(FixedSizeField.flags)
}

