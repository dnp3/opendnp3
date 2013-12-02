package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSizeGroupVariation, ObjectGroup, FixedSizeField}

import FixedSizeField._

object Group11 extends ObjectGroup {
  def objects = List(Group11Var1, Group11Var2)
  def group: Byte = 11
}

object Group11Var1 extends FixedSizeGroupVariation(Group11, 1) {
  def fields = List(flags)
}
object Group11Var2 extends FixedSizeGroupVariation(Group11, 2) {
  def fields = List(flags, time48)
}
