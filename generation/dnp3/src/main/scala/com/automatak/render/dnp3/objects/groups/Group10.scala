package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._

object Group10 extends ObjectGroup {
  def objects = List(Group10Var0, Group10Var1, Group10Var2)
  def group: Byte = 10
}

object Group10Var0 extends AnyVariation(Group10, 0)

object Group10Var1 extends SingleBitfield(Group10, 1)

object Group10Var2 extends FixedSize(Group10, 2)(flags) with ConversionToControlStatus

