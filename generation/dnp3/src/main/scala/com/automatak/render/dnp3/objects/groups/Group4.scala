package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ObjectGroup, FixedSizeGroupVariation}

import com.automatak.render.dnp3.objects.FixedSizeField._

object Group4 extends ObjectGroup {
  def objects = List(Group4Var1, Group4Var2, Group4Var3)
  def group: Byte = 4
}

object Group4Var1 extends FixedSizeGroupVariation(Group4, 1) {
  def fields = List(flags)
}

object Group4Var2 extends FixedSizeGroupVariation(Group4, 2) {
    def fields = List(flags, time48)
}

object Group4Var3 extends FixedSizeGroupVariation(Group4, 3) {
    def fields = List(flags, time16)
}