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
import com.automatak.render.dnp3.objects.generators.ConversionToAnalog

object Group30 extends ObjectGroup {
  def objects = List(Group30Var0, Group30Var1, Group30Var2, Group30Var3, Group30Var4, Group30Var5, Group30Var6)
  def group: Byte = 30
  def desc: String = "Analog Input"
  def isEventGroup: Boolean = false
}

object Group30Var0 extends AnyVariation(Group30, 0)
object Group30Var1 extends FixedSize(Group30, 1, bit32WithFlag)(flags, value32) with ConversionToAnalog with StaticVariation.Analog
object Group30Var2 extends FixedSize(Group30, 2, bit16WithFlag)(flags, value16) with ConversionToAnalog with StaticVariation.Analog
object Group30Var3 extends FixedSize(Group30, 3, bit32WithoutFlag)(value32) with ConversionToAnalog with StaticVariation.Analog
object Group30Var4 extends FixedSize(Group30, 4, bit16WithoutFlag)(value16) with ConversionToAnalog with StaticVariation.Analog
object Group30Var5 extends FixedSize(Group30, 5, singlePrecisionWithFlag)(flags, float32) with ConversionToAnalog with StaticVariation.Analog
object Group30Var6 extends FixedSize(Group30, 6, doublePrecisionWithFlag)(flags, float64) with ConversionToAnalog with StaticVariation.Analog
