package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, ObjectGroup, FixedSizeField}

import FixedSizeField._

object Group11 extends ObjectGroup {
  def objects = List(Group11Var1, Group11Var2)
  def group: Byte = 11
}

object Group11Var1 extends FixedSize(Group11, 1)(flags)
object Group11Var2 extends FixedSize(Group11, 2)(flags, time48)
