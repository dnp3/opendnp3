package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

// counter event
object Group22 extends ObjectGroup {
  def objects = List(Group22Var1, Group22Var2, Group22Var3, Group22Var4, Group22Var5, Group22Var6, Group22Var7, Group22Var8)
  def group: Byte = 22  
}

object Group22Var1 extends FixedSize(Group22, 1)(flags, count32)
object Group22Var2 extends FixedSize(Group22, 2)(flags, count16)
object Group22Var3 extends FixedSize(Group22, 3)(flags, count32) // delta
object Group22Var4 extends FixedSize(Group22, 4)(flags, count16) // delta
object Group22Var5 extends FixedSize(Group22, 5)(flags, count32, time48)
object Group22Var6 extends FixedSize(Group22, 6)(flags, count16, time48)
object Group22Var7 extends FixedSize(Group22, 7)(flags, count32, time48) // delta
object Group22Var8 extends FixedSize(Group22, 8)(flags, count16, time48) // delta
