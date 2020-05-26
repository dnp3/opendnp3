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
package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._

object CppEnumGenerator {

  def apply(enums: List[EnumConfig], cppNamespace : String, incFormatString: String, incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def headerPath(model: EnumModel) = incDirectory.resolve(headerName(model))
    def implPath(model: EnumModel) = implDirectory.resolve(implName(model))
    def privateHeaderPath(model: EnumModel) = implDirectory.resolve(privateHeaderName(model))

    def headerName(model: EnumModel) = model.name + ".h"
    def implName(model: EnumModel) = model.name + ".cpp"
    def privateHeaderName(model: EnumModel) = model.name + "Serialization" + ".h"

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val intConversions = if(cfg.intConv) List(EnumToType, EnumFromType) else Nil
      val stringConversions = if(cfg.stringConv) List(EnumToString, EnumToHumanString, EnumFromString) else Nil

      val renders = intConversions ::: stringConversions

      def writeHeader() : Unit = {
        def license = commented(LicenseHeader())
        def includes = cstdint ++ string
        def enum = EnumModelRenderer.render(cfg.model)
        def spec = struct(f"${cfg.model.name}Spec")(
          Iterator(f"using enum_type_t = ${cfg.model.name};") ++ space ++ renders.flatMap(c => c.header.render(cfg.model)).iterator)
        def lines = license ++ space ++ includeGuards(cfg.model.name)(includes ++ space ++ namespace(cppNamespace)(enum ++ space ++ spec))
        writeTo(headerPath(cfg.model))(lines)
        println("Wrote: " + headerPath(cfg.model))
      }

      def writeImpl(): Unit = {
        def license = commented(LicenseHeader())
        def funcs = renders.flatMap(r => r.impl.render(cfg.model) ++ space).iterator
        def inc = List(quoted(String.format(incFormatString, headerName(cfg.model))), bracketed("stdexcept")).map(i => include(i))
        def lines = license ++ space ++ inc ++ space ++ namespace(cppNamespace)(funcs)

        if(cfg.intConv || cfg.stringConv)
        {
          writeTo(implPath(cfg.model))(lines)
          println("Wrote: " + implPath(cfg.model))
        }
      }

      def writePrivateHeader(): Unit = {
        def license = commented(LicenseHeader())
        def includes = Iterator(include(quoted(String.format(incFormatString, headerName(cfg.model))))) ++ littleEndian
        def funcs = EnumSerialization.render(cfg.model)
        def lines = license ++ space ++ includes ++ space ++ funcs

        if(cfg.serialization)
        {
          val path = privateHeaderPath(cfg.model)
          writeTo(path)(lines)
          println("Wrote: " + path)
        }
      }

      writeHeader()
      writeImpl()
      writePrivateHeader()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
