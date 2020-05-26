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

import com.automatak.render.Indentation
import com.automatak.render.space

import scala.annotation.tailrec

object CppFunction {

  def withSpaces(items: List[Iterator[String]]): Iterator[String] = {
    withSpacesRec(items, Iterator.empty)
  }

  @tailrec
  def withSpacesRec(items: List[Iterator[String]], sum: Iterator[String]): Iterator[String] = items match {
    case x :: tail => if(sum.isEmpty) withSpacesRec(tail, x) else withSpacesRec(tail, sum ++ space ++ x)
    case Nil => sum
  }
}

trait CppFunction {
  def signature(implicit indent: Indentation) : Iterator[String]
  def implementation(implicit indent: Indentation) : Iterator[String]
}
