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
package com.automatak.render.dnp3.objects.groups

import com.automatak.render.dnp3.objects._

import FixedSizeField._
import com.automatak.render.dnp3.objects.VariationNames._
import com.automatak.render.dnp3.objects.generators.ConversionToCounter

// counter event
object Group22 extends ObjectGroup {
  def objects = List(Group22Var0, Group22Var1, Group22Var2, Group22Var5, Group22Var6)
  def group: Byte = 22
  def desc: String = "Counter Event"
  def isEventGroup: Boolean = true
}

object Group22Var0 extends AnyVariation(Group22, 0)
object Group22Var1 extends FixedSize(Group22, 1, bit32WithFlag)(flags, count32) with ConversionToCounter
object Group22Var2 extends FixedSize(Group22, 2, bit16WithFlag)(flags, count16) with ConversionToCounter
object Group22Var5 extends FixedSize(Group22, 5, bit32WithFlagTime)(flags, count32, time48) with ConversionToCounter
object Group22Var6 extends FixedSize(Group22, 6, bit16WithFlagTime)(flags, count16, time48) with ConversionToCounter
