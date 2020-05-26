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
import com.automatak.render.dnp3.objects.generators.ConversionToAnalogOutputStatus

//analog output events
object Group42 extends ObjectGroup {
  def objects = List( Group42Var0, Group42Var1, Group42Var2, Group42Var3, Group42Var4, Group42Var5, Group42Var6, Group42Var7, Group42Var8)
  def group: Byte = 42
  def desc: String = "Analog Output Event"
  def isEventGroup: Boolean = true
}

object Group42Var0 extends AnyVariation(Group42, 0)
object Group42Var1 extends FixedSize(Group42, 1, bit32WithFlag)(flags, value32) with ConversionToAnalogOutputStatus
object Group42Var2 extends FixedSize(Group42, 2, bit16WithFlag)(flags, value16) with ConversionToAnalogOutputStatus
object Group42Var3 extends FixedSize(Group42, 3, bit32WithFlagTime)(flags, value32, time48) with ConversionToAnalogOutputStatus
object Group42Var4 extends FixedSize(Group42, 4, bit16WithFlagTime)(flags, value16, time48) with ConversionToAnalogOutputStatus
object Group42Var5 extends FixedSize(Group42, 5, singlePrecisionWithFlag)(flags, float32) with ConversionToAnalogOutputStatus
object Group42Var6 extends FixedSize(Group42, 6, doublePrecisionWithFlag)(flags, float64) with ConversionToAnalogOutputStatus
object Group42Var7 extends FixedSize(Group42, 7, singlePrecisionWithFlagTime)(flags, float32, time48) with ConversionToAnalogOutputStatus
object Group42Var8 extends FixedSize(Group42, 8, doublePrecisionWithFlagTime)(flags, float64, time48) with ConversionToAnalogOutputStatus