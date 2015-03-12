package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import com.automatak.render.dnp3.objects.FixedSizeField._

object Group122 extends ObjectGroup {
  def objects = List(Group122Var1, Group122Var2)
  def group: Byte = 122
}

object Group122Var1 extends FixedSize(Group122, 1)(flags, assocId, count32)
object Group122Var2 extends FixedSize(Group122, 2)(flags, assocId, count32, time48)
