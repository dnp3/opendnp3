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

object EnumToString {



  def signature(name: String) = List(stdString, List(name,"ToString(", name," arg)").mkString).mkString(" ")

  case class HeaderRender(i: Indentation) extends ModelRenderer[EnumModel] {
    def render(em: EnumModel) : Lines = new Lines {
      def foreach[A](f: String => A): Unit = f(signature(em.name)+";")
    }
  }

  case class ImplRender(i: Indentation) extends ModelRenderer[EnumModel] {
    def render(em: EnumModel) : Lines = new Lines {
      def foreach[A](f: String => A): Unit = {
        f(i.wrap(signature(em.name)))
        bracket(i)(f) {
          f(i.wrap("switch(arg)"))
          bracket(i)(f) {
            em.values.foreach { v =>
              f(i.wrap(List("case(", em.qualified(v),"):").mkString))
              i {
                f(i.wrap(List("return ", quotes(v.name), ";").mkString))
              }
            }
          }
          f(i.wrap(List("return ", quotes(em.default.get.name),";").mkString))
        }
      }
    }
  }
}


