package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

object Group120 extends ObjectGroup {

  /*
  def objects = List(
    Group120Var1,
    Group120Var2,
    Group120Var3,
    Group120Var4,
    Group120Var5,
    Group120Var6,
    Group120Var7,
    Group120Var8,
    Group120Var9,
    Group120Var10,
    Group120Var11,
    Group120Var12,
    Group120Var13,
    Group120Var14,
    Group120Var15
  )
  */

  def objects = List(Group120Var3, Group120Var4)

  def group: Byte = 120
  def desc: String = "Authentication"
}

/*
object Group120Var1 extends VariableSize(Group120, 1, "Challenge")
object Group120Var2 extends VariableSize(Group120, 2, "Reply")
*/

object Group120Var3 extends FixedSize(Group120, 3, "Aggressive Mode Request")(
  FixedSizeField("challengeSeqNum", UInt32Field),
  FixedSizeField("userNum", UInt16Field)
)

object Group120Var4 extends FixedSize(Group120, 4, "Session Key Status Request")(
  FixedSizeField("userNum", UInt16Field)
)

/*
object Group120Var5 extends VariableSize(Group120, 5, "Session Key Status")
object Group120Var6 extends VariableSize(Group120, 6, "Session Key Change")
object Group120Var7 extends VariableSize(Group120, 7, "Error")
object Group120Var8 extends VariableSize(Group120, 8, "User Certificate")
object Group120Var9 extends VariableSize(Group120, 9, "HMAC")
object Group120Var10 extends VariableSize(Group120, 10, "User Status Change")
object Group120Var11 extends VariableSize(Group120, 11, "Update Key Change Request")
object Group120Var12 extends VariableSize(Group120, 12, "Update Key Change Reply")
object Group120Var13 extends VariableSize(Group120, 13, "Update Key Change")
object Group120Var14 extends VariableSize(Group120, 14, "Update Key Change Signature")
object Group120Var15 extends VariableSize(Group120, 15, "Update Key Change Confirmation")
*/