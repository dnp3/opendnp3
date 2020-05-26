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
import com.automatak.render.dnp3.objects.generators.ConversionToTimeAndInterval

// absolute time
object Group50 extends ObjectGroup {
  def objects = List(Group50Var1, Group50Var3, Group50Var4)
  def group: Byte = 50
  def desc: String = "Time and Date"
  def isEventGroup: Boolean = false
}

object Group50Var1 extends FixedSize(Group50, 1, "Absolute Time")(time48)

object Group50Var3 extends FixedSize(Group50, 3, "Absolute Time at last recorded time")(time48)

object Group50Var4 extends FixedSize(Group50, 4, "Indexed absolute time and long interval")(
  time48,
  FixedSizeField("interval", UInt32Field),
  FixedSizeField("units", UInt8Field)
) with ConversionToTimeAndInterval with StaticVariation.TimeAndInterval

