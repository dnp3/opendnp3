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
import com.automatak.render.dnp3.objects.generators.ConversionToBinaryOutputStatus

object Group10 extends ObjectGroup {
  def objects = List(Group10Var0, Group10Var1, Group10Var2)
  def desc: String = "Binary Output"
  def group: Byte = 10
  def isEventGroup: Boolean = false
}

object Group10Var0 extends AnyVariation(Group10, 0)

object Group10Var1 extends SingleBitfield(Group10, 1, packedFormat)

object Group10Var2 extends FixedSize(Group10, 2, outputStatusWithFlags)(flags) with ConversionToBinaryOutputStatus with StaticVariation.BinaryOutputStatus

