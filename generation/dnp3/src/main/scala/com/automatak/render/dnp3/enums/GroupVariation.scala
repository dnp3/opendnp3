package com.automatak.render.dnp3.enums

import com.automatak.render.{EnumValue, Base10, EnumValues, EnumModel}

object GroupVariation {

  private def name(g: Int, v: Int) = List("Group",g.toString,"Var", v.toString).mkString

  private def value(g: Int, v: Int) : Int = g*256 + v

  private def gv(g: Int, v: Int)(desc: String): EnumValue = EnumValue(name(g,v), value(g,v), None, Some(desc))

  // defined using syntactic sugar above
  def enums : List[EnumValue] = List(

    gv(1,0)("Binary Input— Any Variation")




  )
}
