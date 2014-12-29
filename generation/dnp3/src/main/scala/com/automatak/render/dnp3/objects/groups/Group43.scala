package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToAnalogCommandEvent, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

//analog output events
object Group43 extends ObjectGroup {
  def objects = List(Group43Var1, Group43Var2, Group43Var3, Group43Var4, Group43Var5, Group43Var6, Group43Var7, Group43Var8)
  def group: Byte = 43
}

object Group43Var1 extends FixedSize(Group42, 1)(commandStatus, value32) with ConversionToAnalogCommandEvent
object Group43Var2 extends FixedSize(Group42, 2)(commandStatus, value16) with ConversionToAnalogCommandEvent
object Group43Var3 extends FixedSize(Group42, 3)(commandStatus, value32, time48) with ConversionToAnalogCommandEvent
object Group43Var4 extends FixedSize(Group42, 4)(commandStatus, value16, time48) with ConversionToAnalogCommandEvent
object Group43Var5 extends FixedSize(Group42, 5)(commandStatus, float32) with ConversionToAnalogCommandEvent
object Group43Var6 extends FixedSize(Group42, 6)(commandStatus, float64) with ConversionToAnalogCommandEvent
object Group43Var7 extends FixedSize(Group42, 7)(commandStatus, float32, time48) with ConversionToAnalogCommandEvent
object Group43Var8 extends FixedSize(Group42, 8)(commandStatus, float64, time48) with ConversionToAnalogCommandEvent