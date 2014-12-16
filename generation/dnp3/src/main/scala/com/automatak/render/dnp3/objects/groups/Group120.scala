package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

object Group120 extends ObjectGroup {
  def objects = List(Group120Var3)
  def group: Byte = 120
}

object Group120Var3 extends FixedSize(Group120, 3)(
  FixedSizeField("challengeSeqNum", UInt32Field),
  FixedSizeField("userNum", UInt16Field)
)
