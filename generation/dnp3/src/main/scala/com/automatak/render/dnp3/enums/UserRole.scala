/*
 * Copyright 2013-2019 Automatak, LLC
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
package com.automatak.render.dnp3.enums

import com.automatak.render.{Base10, EnumModel, EnumValue}


object UserRole {
  private val comments = List("Enumerates pre-defined rules in secure authentication")
  private val defaultValue = EnumValue("UNDEFINED", 32767)

  def apply(): EnumModel = EnumModel("UserRole", comments, EnumModel.UInt16, codes, Some(defaultValue), Base10)

  private val codes = List(
    EnumValue("VIEWER", 0),
    EnumValue("OPERATOR", 1),
    EnumValue("ENGINEER", 2),
    EnumValue("INSTALLER", 3),
    EnumValue("SECADM", 4),
    EnumValue("SECAUD", 5),
    EnumValue("RBACMNT", 6),
    EnumValue("SINGLE_USER", 32768)
  )
}
