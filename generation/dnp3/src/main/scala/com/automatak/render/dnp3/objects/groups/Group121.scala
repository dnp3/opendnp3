package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import com.automatak.render.dnp3.objects.FixedSizeField._

object Group121 extends ObjectGroup {
  def objects = List(Group121Var1)
  def group: Byte = 121
}

object Group121Var1 extends FixedSize(Group121, 1)(flags, assocId, count32)

