package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects.{AnyVariation, ObjectGroup}

object Group60 extends ObjectGroup {
  def objects = List(Group60Var1, Group60Var2, Group60Var3, Group60Var4)
  def group: Byte = 60
}

object Group60Var1 extends AnyVariation(Group60, 1)
object Group60Var2 extends AnyVariation(Group60, 2)
object Group60Var3 extends AnyVariation(Group60, 3)
object Group60Var4 extends AnyVariation(Group60, 4)
