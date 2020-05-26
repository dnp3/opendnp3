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
import com.automatak.render.dnp3.objects.VariationNames._
import FixedSizeField._
import com.automatak.render.dnp3.objects.generators.ConversionToCounter

// counters
object Group20 extends ObjectGroup {
  def objects = List(Group20Var0, Group20Var1, Group20Var2, Group20Var5, Group20Var6)
  def group: Byte = 20
  def desc: String = "Counter"
  def isEventGroup: Boolean = false
}

object Group20Var0 extends AnyVariation(Group20, 0)
object Group20Var1 extends FixedSize(Group20, 1, bit32WithFlag)(flags, count32) with ConversionToCounter with StaticVariation.Counter
object Group20Var2 extends FixedSize(Group20, 2, bit16WithFlag)(flags, count16) with ConversionToCounter with StaticVariation.Counter
object Group20Var5 extends FixedSize(Group20, 5, bit32WithoutFlag)(count32) with ConversionToCounter with StaticVariation.Counter
object Group20Var6 extends FixedSize(Group20, 6, bit16WithoutFlag)(count16) with ConversionToCounter with StaticVariation.Counter


