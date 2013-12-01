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

    def pair(ir: IntRender)(ev: EnumValue): String = {
      List(ev.name, "=", ir(ev.value)).spaced
    }

    def header: Iterator[String] = Iterator(List("enum","class", enum.name, ":", getType(enum.enumType)).spaced)

    def values : Iterator[String] = {
      val comments: Iterator[Option[String]] = enum.values.map(ev => ev.comment.map(c => "/// " + c)).iterator
      val definitions : Iterator[String] = commaDelimited(enum.values.map(pair(enum.render)).iterator)

      merge(comments, definitions)
    }

    header ++ bracketSemiColon(i) {
      values
    }

  }

}
