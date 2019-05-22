/*
 * Copyright 2013-2019 Automatak, LLC
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
package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import com.automatak.render.dnp3.objects.FixedSizeField._
import com.automatak.render.dnp3.objects.generators.ConversionToSecurityStat

object Group122 extends ObjectGroup {
  def objects = List(Group122Var0, Group122Var1, Group122Var2)
  def group: Byte = 122
  def desc: String = "Security Statistic event"
  def isEventGroup: Boolean = true
}

object Group122Var0 extends AnyVariation(Group122, 0)
object Group122Var1 extends FixedSize(Group122, 1, VariationNames.bit32WithFlag)(flags, assocId, count32) with ConversionToSecurityStat
object Group122Var2 extends FixedSize(Group122, 2, VariationNames.bit32WithFlagTime)(flags, assocId, count32, time48) with ConversionToSecurityStat
