package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToCounter, FixedSize, ObjectGroup, FixedSizeField}

import FixedSizeField._

// counters
object Group20 extends ObjectGroup {
  def objects = List(Group20Var1, Group20Var2, Group20Var3, Group20Var4, Group20Var5, Group20Var6, Group20Var7, Group20Var8)
  def group: Byte = 20
}

object Group20Var1 extends FixedSize(Group20, 1)(flags, count32) with ConversionToCounter
object Group20Var2 extends FixedSize(Group20, 2)(flags, count16) with ConversionToCounter
object Group20Var3 extends FixedSize(Group20, 3)(flags, count32) with ConversionToCounter
object Group20Var4 extends FixedSize(Group20, 4)(flags, count16) with ConversionToCounter
object Group20Var5 extends FixedSize(Group20, 5)(count32) with ConversionToCounter
object Group20Var6 extends FixedSize(Group20, 6)(count16) with ConversionToCounter
object Group20Var7 extends FixedSize(Group20, 7)(count32) with ConversionToCounter
object Group20Var8 extends FixedSize(Group20, 8)(count16) with ConversionToCounter


