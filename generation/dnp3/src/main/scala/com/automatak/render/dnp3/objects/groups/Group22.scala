package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToCounter, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

// counter event
object Group22 extends ObjectGroup {
  def objects = List(Group22Var1, Group22Var2, Group22Var5, Group22Var6)
  def group: Byte = 22  
}

object Group22Var1 extends FixedSize(Group22, 1)(flags, count32) with ConversionToCounter
object Group22Var2 extends FixedSize(Group22, 2)(flags, count16) with ConversionToCounter
object Group22Var5 extends FixedSize(Group22, 5)(flags, count32, time48) with ConversionToCounter
object Group22Var6 extends FixedSize(Group22, 6)(flags, count16, time48) with ConversionToCounter
