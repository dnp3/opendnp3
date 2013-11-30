package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSizeField, AnyVariation, FixedSizeGroupVariation, ObjectGroup}

import FixedSizeField._

object Group2 extends ObjectGroup {
  val objects = List(Group2Var0, Group2Var1, Group2Var2, Group2Var3)
  def group: Byte = 2
}

object Group2Var0 extends AnyVariation(Group2, 0)

object Group2Var1 extends FixedSizeGroupVariation(Group2, 1) {
  def fields = List(flags)
}

object Group2Var2 extends FixedSizeGroupVariation(Group2, 2) {
  def fields = List(flags, time48)
}

object Group2Var3 extends FixedSizeGroupVariation(Group2, 3) {
  def fields = List(flags, time16)
}