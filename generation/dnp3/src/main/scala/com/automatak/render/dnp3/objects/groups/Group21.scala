package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, ObjectGroup, FixedSizeField}

import FixedSizeField._

// frozen counters
object Group21 extends ObjectGroup {

  def objects = List(
    Group21Var1,
    Group21Var2,
    Group21Var3,
    Group21Var4,
    Group21Var5,
    Group21Var6,
    Group21Var7,
    Group21Var8,
    Group21Var9,
    Group21Var10,
    Group21Var11,
    Group21Var12)

  def group: Byte = 21

}

object Group21Var1 extends FixedSize(Group21, 1)(flags, count32)
object Group21Var2 extends FixedSize(Group21, 2)(flags, count16)
object Group21Var3 extends FixedSize(Group21, 3)(flags, count32) // delta
object Group21Var4 extends FixedSize(Group21, 4)(flags, count16) // delta
object Group21Var5 extends FixedSize(Group21, 5)(flags, count32, time48)
object Group21Var6 extends FixedSize(Group21, 6)(flags, count16, time48)
object Group21Var7 extends FixedSize(Group21, 7)(flags, count32, time48) // delta
object Group21Var8 extends FixedSize(Group21, 8) (flags, count16, time48) // delta
object Group21Var9 extends FixedSize(Group21, 9)(count16)
object Group21Var10 extends FixedSize(Group21, 10)(count32)
object Group21Var11 extends FixedSize(Group21, 11)(count16) // delta
object Group21Var12 extends FixedSize(Group21, 12)(count32) // delta