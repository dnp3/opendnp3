package com.automatak.render.dnp3.enums

import com.automatak.render.{EnumValue, Base10, EnumValues, EnumModel}

object EventTypes {

  private def create(name: String, values: List[EnumValue]): EnumModel = EnumModel(name, Nil, EnumModel.Integer, values, Base10)

  private def group(i: Int)(vars: Int*): List[EnumValue] = EnumValues.from(vars.map(v => List("Group",i.toString,"Var", v.toString).mkString).toList)

  // defined using syntactic sugar above
  def enums : List[EnumModel] = List(
    create("StaticBinaryResponse", group(1)(2)),
    create("StaticAnalogResponse", group(30)(1,2,3,4,5,6)),
    create("StaticCounterResponse", group(20)(1,2,5,6)),
    create("StaticFrozenCounterResponse", group(21)(1,2,5,6,7,8,9,10)),
    create("StaticSetpointStatusResponse",group(40)(1,2,3,4)),
    create("EventBinaryResponse", group(2)(1,2)),
    create("EventAnalogResponse", group(32)(1,2,3,4,5,6,7,8)),
    create("EventCounterResponse", group(22)(1,2,5,6))
  )
}
