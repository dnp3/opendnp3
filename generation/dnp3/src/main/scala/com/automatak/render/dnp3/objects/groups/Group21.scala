package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToFrozenCounter, FixedSize, ObjectGroup, FixedSizeField}

import FixedSizeField._

// frozen counters
object Group21 extends ObjectGroup {

  def objects = List(
    Group21Var1,
    Group21Var2,
    Group21Var5,
    Group21Var6,
    Group21Var9,
    Group21Var10
  )

  def group: Byte = 21

}

object Group21Var1 extends FixedSize(Group21, 1)(flags, count32) with ConversionToFrozenCounter
object Group21Var2 extends FixedSize(Group21, 2)(flags, count16) with ConversionToFrozenCounter
object Group21Var5 extends FixedSize(Group21, 5)(flags, count32, time48) with ConversionToFrozenCounter
object Group21Var6 extends FixedSize(Group21, 6)(flags, count16, time48) with ConversionToFrozenCounter
object Group21Var9 extends FixedSize(Group21, 9)(count16) with ConversionToFrozenCounter
object Group21Var10 extends FixedSize(Group21, 10)(count32) with ConversionToFrozenCounter
