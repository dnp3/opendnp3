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

object EnumFromString extends HeaderImplModelRender[EnumModel] {

  def header: ModelRenderer[EnumModel] = HeaderRender
  def impl: ModelRenderer[EnumModel]  = ImplRender

  private def signatureReturnType(enum: EnumModel) = enum.name
  private def signatureMethod = "from_string(const std::string& arg)"

  private object HeaderRender extends ModelRenderer[EnumModel] {
    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {
      Iterator(toHeaderSignature(signatureReturnType(em), signatureMethod))
    }
  }

  private object ImplRender extends ModelRenderer[EnumModel] {

    def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {

      def header = Iterator(toImplSignature(signatureReturnType(em), signatureMethod, em))
      def nonDefaults : Iterator[String] = {
        em.nonDefaultValues.map(c => {
          s"""if(arg == "${c.name}") return ${em.name}::${c.name};"""
        }).iterator
      }

      def default: Iterator[String] = {
        em.defaultValue match {
          case Some(value) => Iterator(s"else return ${em.qualified(value)};")
          case None => Iterator("""else throw std::invalid_argument(std::string("Unknown value: ") + arg);""")
        }
      }

      header ++ bracket {
        nonDefaults ++ default
      }
    }
  }
}


