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

import com.automatak.render.dnp3.objects.{ClassData, AnyVariation, ObjectGroup}

object Group60 extends ObjectGroup {
  def objects = List(Group60Var1, Group60Var2, Group60Var3, Group60Var4)
  def group: Byte = 60
  def desc: String = "Class Data"
  def isEventGroup: Boolean = false
}

object Group60Var1 extends ClassData(Group60, 1, "Class 0")
object Group60Var2 extends ClassData(Group60, 2, "Class 1")
object Group60Var3 extends ClassData(Group60, 3, "Class 2")
object Group60Var4 extends ClassData(Group60, 4, "Class 3")
