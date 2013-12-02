package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, ObjectGroup}

import com.automatak.render.dnp3.objects.FixedSizeField._

object Group4 extends ObjectGroup {
  def objects = List(Group4Var1, Group4Var2, Group4Var3)
  def group: Byte = 4
}

object Group4Var1 extends FixedSize(Group4, 1)(flags)
object Group4Var2 extends FixedSize(Group4, 2)(flags, time48)
object Group4Var3 extends FixedSize(Group4, 3)(flags, time16)