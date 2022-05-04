/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Step Function I/O LLC license
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

object EnumModelRenderer extends ModelRenderer[EnumModel] {

  def render(enum: EnumModel)(implicit indent: Indentation) : Iterator[String] = {

    def pair(ir: IntRender)(ev: EnumValue): String = List(ev.name, "=", ir(ev.value)).spaced

    def header: Iterator[String] = Iterator(List("enum","class", enum.name, ":", getEnumType(enum.enumType)).spaced)

    def comments: Iterator[Option[Iterator[String]]] = enum.allValues.map(ev => ev.comment.map(c => Iterator("/// " + c))).iterator

    def definitions : Iterator[String] = commaDelimited(enum.allValues.map(pair(enum.render)).iterator)

    def summary = if(enum.comments.isEmpty) Iterator.empty else {
      Iterator("/**") ++ indent(enum.comments.iterator) ++ Iterator("*/")
    }

    summary ++
    header ++ bracketSemiColon {
      merge(comments, definitions)
    }

  }

}
