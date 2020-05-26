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
import com.automatak.render.dnp3.objects.generators.ConversionToBinary

object Group2 extends ObjectGroup {
  val objects = List(Group2Var0, Group2Var1, Group2Var2, Group2Var3)
  def group: Byte = 2
  def desc: String = "Binary Input Event"
  def isEventGroup: Boolean = true
}

object Group2Var0 extends AnyVariation(Group2, 0)
object Group2Var1 extends FixedSize(Group2, 1, withoutTime)(flags) with ConversionToBinary
object Group2Var2 extends FixedSize(Group2, 2, withAbsoluteTime)(flags, time48) with ConversionToBinary
object Group2Var3 extends FixedSize(Group2, 3, withRelativeTime)(flags, time16) with ConversionToBinary