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
package com.automatak.render.cpp

import com.automatak.render._

class EnumModelRenderer(i: Indentation) extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel) : Iterator[String] = {

    def toString(ir: IntRender)(ev: EnumValue): String = {
      List(Some(ev.name), ev.value.map(x => "="), ev.value.map(ir.apply)).flatten.spaced
    }

    def header: Iterator[String] = {
      val list : List[Option[String]] = List("enum","class", enum.name).map(x => Some(x)) ::: List(enum.enumType.map(x => ":"), enum.enumType.map(getType))
      Iterator(list.flatten.spaced)
    }

    def values = enum.values.map(toString(enum.render)).commaDelimited.toIterator

    header ++ bracketSemiColon(i) {
      values
    }

  }

}
