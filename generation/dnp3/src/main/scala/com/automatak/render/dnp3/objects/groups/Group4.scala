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

import com.automatak.render.dnp3.objects.generators.ConversionToDoubleBitBinary
import com.automatak.render.dnp3.objects.{AnyVariation, FixedSize, ObjectGroup}

import com.automatak.render.dnp3.objects.FixedSizeField._
import com.automatak.render.dnp3.objects.VariationNames._

object Group4 extends ObjectGroup {
  def objects = List(Group4Var0, Group4Var1, Group4Var2, Group4Var3)
  def group: Byte = 4
  def desc: String = "Double-bit Binary Input Event"
  def isEventGroup: Boolean = true
}

object Group4Var0 extends AnyVariation(Group4, 0)
object Group4Var1 extends FixedSize(Group4, 1, withoutTime)(flags) with ConversionToDoubleBitBinary
object Group4Var2 extends FixedSize(Group4, 2, withAbsoluteTime)(flags, time48) with ConversionToDoubleBitBinary
object Group4Var3 extends FixedSize(Group4, 3, withRelativeTime)(flags, time16) with ConversionToDoubleBitBinary