package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

// common time of occurrence
object Group51 extends ObjectGroup {
  def objects = List(Group51Var1, Group51Var2)
  def group: Byte = 51
}

object Group51Var1 extends FixedSize(Group51, 1)(time48)
object Group51Var2 extends FixedSize(Group51, 2)(time48)
