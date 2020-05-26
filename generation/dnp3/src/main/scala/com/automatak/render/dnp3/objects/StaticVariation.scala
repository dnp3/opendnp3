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
package com.automatak.render.dnp3.objects

import com.automatak.render.Indentation

trait StaticVariation extends GroupVariation {
  def staticEnumName : String
  override def headerLines(implicit i : Indentation) : Iterator[String] = {
    super.headerLines ++ Iterator("static const %s svariation = %s::Group%dVar%d;".format(staticEnumName, staticEnumName, group, variation))
  }
}

object StaticVariation {

  trait Binary extends StaticVariation {
    override def staticEnumName : String = "StaticBinaryVariation"
  }

  trait DoubleBinary extends StaticVariation {
    override def staticEnumName : String = "StaticDoubleBinaryVariation"
  }

  trait Analog extends StaticVariation {
    override def staticEnumName : String = "StaticAnalogVariation"
  }

  trait Counter extends StaticVariation {
    override def staticEnumName : String = "StaticCounterVariation"
  }

  trait FrozenCounter extends StaticVariation {
    override def staticEnumName : String = "StaticFrozenCounterVariation"
  }

  trait BinaryOutputStatus extends StaticVariation {
    override def staticEnumName : String = "StaticBinaryOutputStatusVariation"
  }

  trait AnalogOutputStatus extends StaticVariation {
    override def staticEnumName : String = "StaticAnalogOutputStatusVariation"
  }

  trait TimeAndInterval extends StaticVariation {
    override def staticEnumName : String = "StaticTimeAndIntervalVariation"
  }
}

