/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

// Analog output status
object Group40 extends ObjectGroup {
  def objects = List(Group40Var0, Group40Var1, Group40Var2, Group40Var3, Group40Var4)
  def group: Byte = 40
  def desc: String = "Analog Output Status"
  def isEventGroup: Boolean = false
}

object Group40Var0 extends AnyVariation(Group40, 0)
object Group40Var1 extends FixedSize(Group40, 1, bit32WithFlag)(flags, value32) with ConversionToAnalogOutputStatus with StaticVariation.AnalogOutputStatus
object Group40Var2 extends FixedSize(Group40, 2, bit16WithFlag)(flags, value16) with ConversionToAnalogOutputStatus with StaticVariation.AnalogOutputStatus
object Group40Var3 extends FixedSize(Group40, 3, singlePrecisionWithFlag)(flags, float32) with ConversionToAnalogOutputStatus with StaticVariation.AnalogOutputStatus
object Group40Var4 extends FixedSize(Group40, 4, doublePrecisionWithFlag)(flags, float64) with ConversionToAnalogOutputStatus with StaticVariation.AnalogOutputStatus
