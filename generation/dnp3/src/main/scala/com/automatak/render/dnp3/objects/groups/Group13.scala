package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{ConversionToBinaryCommandEvent, FixedSize, FixedSizeField, ObjectGroup}

import FixedSizeField._

object Group13 extends ObjectGroup {
  def objects = List(Group13Var1, Group13Var2)
  def group: Byte = 13
}

object Group13Var1 extends FixedSize(Group13, 1)(flags) with ConversionToBinaryCommandEvent
object Group13Var2 extends FixedSize(Group13, 2)(flags, time48) with ConversionToBinaryCommandEvent