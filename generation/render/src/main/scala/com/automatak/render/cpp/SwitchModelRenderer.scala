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
package com.automatak.render.cpp

import com.automatak.render._

/*
object DefaultSwitchModelRenderer {

  def wrap[A](f: A => String): A => String = { (a: A) => "return " + f(a) + ";" }

}

class DefaultSwitchModelRenderer[A](fCase: A => String)(fAction: A => String) extends SwitchModelRenderer(fCase)(DefaultSwitchModelRenderer.wrap(fAction))
*/

class SwitchModelRenderer[A](fCase: A => String)(fAction: A => String) {

  def render(nonDefaults: List[A], default: Option[A])(implicit indent: Indentation): Iterator[String] = {

    def switch = Iterator("switch(arg)")

    def nonDefaultCases: Iterator[String] = nonDefaults.iterator.flatMap { c =>
      Iterator(List("case(", fCase(c), "):").mkString) ++
        indent {
          Iterator("return " + fAction(c) + ";")
        }
    }

    def defaultCase: Iterator[String] = default match {
      case Some(value) => Iterator("default:") ++ indent { Iterator("return " + fAction(value) + ";") }
      case None => Iterator("default:") ++ indent { Iterator(s"""throw new std::invalid_argument("Unknown value");""") }
    }

    switch ++ bracket {
      nonDefaultCases ++ defaultCase
    }
  }
}
