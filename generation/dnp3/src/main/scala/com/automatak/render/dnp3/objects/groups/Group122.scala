package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import com.automatak.render.dnp3.objects.FixedSizeField._

object Group122 extends ObjectGroup {
  def objects = List(Group122Var1, Group122Var2)
  def group: Byte = 122
  def desc: String = "Security Statistic event"
}

object Group122Var1 extends FixedSize(Group122, 1, ObjectNames.bit32WithFlag)(flags, assocId, count32)
object Group122Var2 extends FixedSize(Group122, 2, ObjectNames.bit32WithFlagTime)(flags, assocId, count32, time48)
