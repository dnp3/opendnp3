package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._

object Group12 extends ObjectGroup {
  def objects = List()//Group12Var1, Group12Var2, Group12Var3)
  def group: Byte = 12
}

object Group12Var1 extends FixedSizeGroupVariation(Group12, 1) {
  def fields = List(
    controlCode,
    FixedSizeField("count", UInt8Field),
    FixedSizeField("onTime", UInt32Field),
    FixedSizeField("offTime", UInt32Field),
    commandStatus
  )
}


//object Group12Var2 extends FixedSizeGroupVariation(Group12, 2, 11)


//object Group12Var3 extends BasicGroupVariation(Group12, 3)
