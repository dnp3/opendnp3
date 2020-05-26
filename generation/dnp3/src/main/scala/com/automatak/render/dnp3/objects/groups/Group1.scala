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
import com.automatak.render.dnp3.objects.generators.ConversionToBinary

object Group1 extends ObjectGroup {
  def objects = List(Group1Var0, Group1Var1, Group1Var2)
  def desc: String = "Binary Input"
  def group: Byte = 1
  def isEventGroup: Boolean = false
}

object Group1Var0 extends AnyVariation(Group1, 0)

object Group1Var1 extends SingleBitfield(Group1, 1, packedFormat) with StaticVariation.Binary

object Group1Var2 extends FixedSize(Group1, 2, withFlags)(FixedSizeField.flags) with ConversionToBinary with StaticVariation.Binary

