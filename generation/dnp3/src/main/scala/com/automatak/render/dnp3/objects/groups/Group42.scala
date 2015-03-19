package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToAnalogOutputStatus, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._
import com.automatak.render.dnp3.objects.ObjectNames._

//analog output events
object Group42 extends ObjectGroup {
  def objects = List(Group42Var1, Group42Var2, Group42Var3, Group42Var4, Group42Var5, Group42Var6, Group42Var7, Group42Var8)
  def group: Byte = 42
  def desc: String = "Analog Output Event"
}

object Group42Var1 extends FixedSize(Group42, 1, bit32WithFlag)(flags, value32) with ConversionToAnalogOutputStatus
object Group42Var2 extends FixedSize(Group42, 2, bit16WithFlag)(flags, value16) with ConversionToAnalogOutputStatus
object Group42Var3 extends FixedSize(Group42, 3, bit32WithFlagTime)(flags, value32, time48) with ConversionToAnalogOutputStatus
object Group42Var4 extends FixedSize(Group42, 4, bit16WithFlagTime)(flags, value16, time48) with ConversionToAnalogOutputStatus
object Group42Var5 extends FixedSize(Group42, 5, singlePrecisionWithFlag)(flags, float32) with ConversionToAnalogOutputStatus
object Group42Var6 extends FixedSize(Group42, 6, doublePrecisionWithFlag)(flags, float64) with ConversionToAnalogOutputStatus
object Group42Var7 extends FixedSize(Group42, 7, singlePrecisionWithFlagTime)(flags, float32, time48) with ConversionToAnalogOutputStatus
object Group42Var8 extends FixedSize(Group42, 8, doublePrecisionWithFlagTime)(flags, float64, time48) with ConversionToAnalogOutputStatus