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
package com.automatak.render

package object cpp {

  private val quote : String = "\""

  def quotes(s: String): String = List(quote, s, quote).mkString

  def stdString: String = "std::string"

  def getType(typ: EnumModel.Type): String = typ match {
    case EnumModel.UInt8 => "uint8_t"
    case EnumModel.UInt16 => "uint16_t"
  }

  def commented(lines: List[String]): List[String] = {
    lines.map(l => "// " + l)
  }

  def namespace(ns: String)(internals: Iterator[String]): Iterator[String] = {

    Iterator.apply(List("namespace",ns,"{").spaced) ++
    space ++
    internals ++
    space ++
    Iterator.apply("}")

  }

  def includeGuards(name: String)(internals: Iterator[String]): Iterator[String] = {

    val pattern = "__OPENDNP3_GENERATED_"+ name.toUpperCase + "_H_"

    Iterator.apply("#ifndef " + pattern) ++
    Iterator.apply("#define " + pattern) ++
    space ++
    internals ++
    space ++
    Iterator.apply("#endif")

  }

  def staticCast(typ: EnumModel.Type)(arg: String): String = staticCast(getType(typ))(arg)

  def staticCast(typ: String)(arg: String): String = List("static_cast<",typ,">(",arg,")").mkString

  def include(s: String): String = "#include " + s

  def cstdint : Iterator[String] = Iterator.apply(include("<cstdint>"))

  def string : Iterator[String] = Iterator.apply(include("<string>"))

}
