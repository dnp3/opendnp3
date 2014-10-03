package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._

// absolute time
object Group50 extends ObjectGroup {
  def objects = List(Group50Var1, Group50Var4)
  def group: Byte = 50
}

object Group50Var1 extends FixedSize(Group50, 1)(time48)
object Group50Var4 extends FixedSize(Group50, 4)(time48, FixedSizeField("interval", UInt32Field), FixedSizeField("units", UInt8Field))

/*
object Group50Var2 extends FixedSize(Group50, 2)
object Group50Var3 extends FixedSize(Group50, 3)
object Group50Var4 extends FixedSize(Group50, 4)
*/
