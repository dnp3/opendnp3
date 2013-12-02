package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

//analog output events
object Group42 extends ObjectGroup {
  def objects = List(Group42Var1, Group42Var2, Group42Var3, Group42Var4, Group42Var5, Group42Var6, Group42Var7, Group42Var8)
  def group: Byte = 42
}

object Group42Var1 extends FixedSize(Group42, 1)(flags, value32)
object Group42Var2 extends FixedSize(Group42, 2)(flags, value16)
object Group42Var3 extends FixedSize(Group42, 3)(flags, value32, time48)
object Group42Var4 extends FixedSize(Group42, 4)(flags, value16, time48)
object Group42Var5 extends FixedSize(Group42, 5)(flags, float32)
object Group42Var6 extends FixedSize(Group42, 6)(flags, float64)
object Group42Var7 extends FixedSize(Group42, 7)(flags, float32, time48)
object Group42Var8 extends FixedSize(Group42, 8)(flags, float64, time48)