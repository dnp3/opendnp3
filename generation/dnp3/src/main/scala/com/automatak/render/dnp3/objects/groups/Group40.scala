package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToAnalogOutputStatus, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

// Analog output status
object Group40 extends ObjectGroup {
  def objects = List(Group40Var1, Group40Var2, Group40Var3, Group40Var4)
  def group: Byte = 40
}

object Group40Var1 extends FixedSize(Group40, 1)(flags, value32) with ConversionToAnalogOutputStatus
object Group40Var2 extends FixedSize(Group40, 2)(flags, value16) with ConversionToAnalogOutputStatus
object Group40Var3 extends FixedSize(Group40, 3)(flags, float32) with ConversionToAnalogOutputStatus
object Group40Var4 extends FixedSize(Group40, 4)(flags, float64) with ConversionToAnalogOutputStatus
