/*
 * Copyright 2013-2019 Automatak, LLC
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

    def headerName(model: EnumModel) = model.name + ".h"
    def implName(model: EnumModel) = model.name + ".cpp"

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val conversions = if(cfg.conversions) List(EnumToType, EnumFromType) else Nil
      val conversionsOutsideNamespace = if(cfg.conversions) List(EnumSerialization) else Nil
      val stringify = if(cfg.stringConv) List(EnumToString) else Nil

      val renders = conversions ::: stringify

      def writeHeader() {
        def license = commented(LicenseHeader())
        def includes = cstdint ++ { if(cfg.conversions) littleEndian else Iterator.empty }
        def enum = EnumModelRenderer.render(cfg.model)
        def signatures = renders.flatMap(c => c.header.render(cfg.model))
        def signaturesOutsideNamespace = conversionsOutsideNamespace.flatMap(c => c.header.render(cfg.model))
        def lines = license ++ space ++ includeGuards(cfg.model.name)(includes ++ space ++ namespace(cppNamespace)(enum ++ space ++ signatures) ++ space ++ signaturesOutsideNamespace)
        writeTo(headerPath(cfg.model))(lines)
        println("Wrote: " + headerPath(cfg.model))
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.flatMap(r => r.impl.render(cfg.model)).toIterator
        def funcsOutsidenamespace = conversionsOutsideNamespace.flatMap(r => r.impl.render(cfg.model))
        def inc = quoted(String.format(incFormatString, headerName(cfg.model)))
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(cppNamespace)(funcs) ++ space ++ funcsOutsidenamespace

        if(cfg.conversions || cfg.stringConv)
        {
          writeTo(implPath(cfg.model))(lines)
          println("Wrote: " + implPath(cfg.model))
        }
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
