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
import com.automatak.render.dnp3.objects.generators.ConversionToFrozenCounter

// frozen counters
object Group21 extends ObjectGroup {

  def objects = List(
    Group21Var0,
    Group21Var1,
    Group21Var2,
    Group21Var5,
    Group21Var6,
    Group21Var9,
    Group21Var10
  )

  def group: Byte = 21
  def desc: String = "Frozen Counter"
  def isEventGroup: Boolean = false

}

object Group21Var0 extends AnyVariation(Group21, 0)
object Group21Var1 extends FixedSize(Group21, 1, bit32WithFlag)(flags, count32) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
object Group21Var2 extends FixedSize(Group21, 2, bit16WithFlag)(flags, count16) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
object Group21Var5 extends FixedSize(Group21, 5, bit32WithFlagTime)(flags, count32, time48) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
object Group21Var6 extends FixedSize(Group21, 6, bit16WithFlagTime)(flags, count16, time48) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
object Group21Var9 extends FixedSize(Group21, 9, bit32WithoutFlag)(count32) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
object Group21Var10 extends FixedSize(Group21, 10,bit16WithoutFlag)(count16) with ConversionToFrozenCounter with StaticVariation.FrozenCounter
