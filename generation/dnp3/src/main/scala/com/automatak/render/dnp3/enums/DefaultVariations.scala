/**
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.render.dnp3.enums

import com.automatak.render.{EnumValue, Base10, EnumValues, EnumModel}

object DefaultVariations {

  private def create(name: String, values: List[EnumValue]): EnumModel = EnumModel(name, Nil, EnumModel.UInt8, values, None, Base10)

  private def group(i: Int)(vars: Int*): List[EnumValue] = EnumValues.from(vars.map(v => List("Group",i.toString,"Var", v.toString).mkString).toList)

  // defined using syntactic sugar above
  def enums : List[EnumModel] = List(

    create("StaticBinaryVariation", group(1)(1,2)),
    create("StaticDoubleBinaryVariation", group(3)(2)),
    create("StaticAnalogVariation", group(30)(1,2,3,4,5,6)),
    create("StaticCounterVariation", group(20)(1,2,5,6)),
    create("StaticFrozenCounterVariation", group(21)(1,2,5,6,9,10)),
    create("StaticBinaryOutputStatusVariation",group(10)(2)),
    create("StaticAnalogOutputStatusVariation",group(40)(1,2,3,4)),
    create("StaticTimeAndIntervalVariation", group(50)(4)),
    create("StaticOctetStringVariation", group(110)(0)),
    create("StaticSecurityStatVariation", group(121)(1)),

    create("EventBinaryVariation", group(2)(1,2,3)),
    create("EventDoubleBinaryVariation", group(4)(1,2,3)),
    create("EventAnalogVariation", group(32)(1,2,3,4,5,6,7,8)),
    create("EventCounterVariation", group(22)(1,2,5,6)),
    create("EventFrozenCounterVariation", group(23)(1,2,5,6)),
    create("EventBinaryOutputStatusVariation",group(11)(1,2)),
    create("EventAnalogOutputStatusVariation",group(42)(1,2,3,4,5,6,7,8)),
    create("EventOctetStringVariation", group(111)(0)),
    create("EventSecurityStatVariation", group(122)(1,2))

  )
}
