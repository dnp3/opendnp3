package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import com.automatak.render.dnp3.objects.FixedSizeField._
import com.automatak.render.dnp3.objects.VariableFields._

object Group120 extends ObjectGroup {

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

  def group: Byte = 120
  def desc: String = "Authentication"
}

object Group120Var1 extends AuthVariableSize(Group120, 1, "Challenge",
  List(csq, user, macAlgo, challengeReason),
  Nil,
  Some(VariableFields.challengeData)
)


object Group120Var2 extends AuthVariableSize(Group120, 2, "Reply",
  List(csq, user),
  Nil,
  Some(hmac)
)

object Group120Var3 extends FixedSize(Group120, 3, "Aggressive Mode Request")(csq, user)

object Group120Var4 extends FixedSize(Group120, 4, "Session Key Status Request")(user)

object Group120Var5 extends AuthVariableSize(Group120, 5, "Session Key Status",
  List(ksq, user, keyWrapAlgo, keyStatus, macAlgo),
  List(challengeData),
  Some(hmac)
)


object Group120Var6 extends AuthVariableSize(Group120, 6, "Session Key Change",
  List(ksq,user),
  Nil,
  Some(keyWrapData)
)

object Group120Var7 extends AuthVariableSize(Group120, 7, "Error",
  List(csq, user, assocId, errorCode, time48),
  Nil,
  Some(errorText)
)

object Group120Var8 extends AuthVariableSize(Group120, 8, "User Certificate",
  List(keyChangeMethod, certificateType),
  Nil,
  Some(certificate)
)

object Group120Var9 extends AuthVariableSize(Group120, 9, "HMAC", Nil, Nil, Some(hmac))

object Group120Var10 extends AuthVariableSize(Group120, 10, "User Status Change",
  List(keyChangeMethod, userOperation, scsq, userRole, userRoleExpDays),
  List(userName, userPublicKey, certificationData),
  None
)


object Group120Var11 extends DefaultVariableSize(Group120, 11, "Update Key Change Request")
object Group120Var12 extends DefaultVariableSize(Group120, 12, "Update Key Change Reply")
object Group120Var13 extends DefaultVariableSize(Group120, 13, "Update Key Change")
object Group120Var14 extends DefaultVariableSize(Group120, 14, "Update Key Change Signature")
object Group120Var15 extends DefaultVariableSize(Group120, 15, "Update Key Change Confirmation")