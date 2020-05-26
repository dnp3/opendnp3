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
import com.automatak.render.dnp3.objects.generators.{ConversionToAnalogOutputDouble64, ConversionToAnalogOutputFloat32, ConversionToAnalogOutputInt32, ConversionToAnalogOutputInt16}


object Group41 extends ObjectGroup {
  def objects = List(Group41Var0, Group41Var1, Group41Var2, Group41Var3, Group41Var4)
  def group: Byte = 41
  def desc: String = "Analog Output"
  def isEventGroup: Boolean = false
}

object Group41Var0 extends AnyVariation(Group41, 0)
object Group41Var1 extends FixedSize(Group41, 1, bit32WithFlag)(value32, commandStatus) with ConversionToAnalogOutputInt32
object Group41Var2 extends FixedSize(Group41, 2, bit16WithFlag)(value16, commandStatus) with ConversionToAnalogOutputInt16
object Group41Var3 extends FixedSize(Group41, 3, singlePrecision)(float32, commandStatus) with ConversionToAnalogOutputFloat32
object Group41Var4 extends FixedSize(Group41, 4, doublePrecision)(float64, commandStatus) with ConversionToAnalogOutputDouble64