package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._

object Group12 extends ObjectGroup {
  def objects = List(Group12Var1)
  def group: Byte = 12
}

object Group12Var1 extends FixedSize(Group12, 1)(
  FixedSizeField("code", UInt8Field),
  FixedSizeField("count", UInt8Field),
  FixedSizeField("onTime", UInt32Field),
  FixedSizeField("offTime", UInt32Field),
  commandStatus
) with ConversionToCROB
