/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak.render



object EnumValue {
  def apply(name: String, value: Int) : EnumValue = EnumValue(name, Some(value))
  def apply(name: String) : EnumValue = EnumValue(name, None)
}

case class EnumValue(name: String, value: Option[Int])


object EnumModel {

  sealed trait Type
  case object UInt8 extends Type
  case object UInt16 extends Type

}

sealed trait IntRender { def apply(i: Int): String }
case object Hex extends IntRender { def apply(i: Int): String = "0x"+Integer.toHexString(i).toUpperCase }
case object Base10 extends IntRender { def apply(i: Int): String = i.toString }


case class EnumModel(name: String, enumType: Option[EnumModel.Type], values: List[EnumValue], default: Option[EnumValue], render: IntRender = Base10) {
  def qualified(ev: EnumValue): String = List(name,"::",ev.name).mkString
}
