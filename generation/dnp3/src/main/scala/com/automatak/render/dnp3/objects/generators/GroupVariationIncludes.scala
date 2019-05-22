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
package com.automatak.render.dnp3.objects.generators

object GroupVariationIncludes {

   def headerReadWrite : Iterator[String] = Iterator(
     "<openpal/container/RSlice.h>",
     "<openpal/container/WSlice.h>",
     """"opendnp3/app/DNPTime.h""""
   )

  def implReadWrite : Iterator[String] = Iterator(
    "<openpal/serialization/Format.h>",
    "<openpal/serialization/Parse.h>"
  )

  def headerVariableLength: Iterator[String] = Iterator(
    """"opendnp3/app/IVariableLength.h""""
  )

  def implVariableLength: Iterator[String] = Iterator(
    "<openpal/serialization/Serialization.h>",
    """"opendnp3/app/parsing/PrefixFields.h""""
  )

 }
