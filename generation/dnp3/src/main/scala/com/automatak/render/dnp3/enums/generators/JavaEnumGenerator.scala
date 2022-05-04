/**
 * Copyright 2013-2022 Step Function I/O, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Step Function I/O
 * LLC (https://stepfunc.io) under one or more contributor license agreements.
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
package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._

import com.automatak.render.cpp.CppIndentation
import com.automatak.render.java.EnumModelRenderer

object JavaEnumGenerator {

  def apply(enums: List[EnumModel], packageLine: String, dir: Path): Unit = {

    def fileName(model: EnumModel) = model.name + ".java"

    def filePath(model: EnumModel) = dir.resolve(fileName(model))

    implicit val indent = CppIndentation()

    def writeEnumToFile(model: EnumModel): Unit = {
      def license = commented(LicenseHeader())
      def enum = EnumModelRenderer.render(model)
      def lines = license ++ space ++ Iterator(packageLine) ++ (enum)
      writeTo(filePath(model))(lines)
      println("Wrote: " + filePath(model))
    }

    enums.foreach(writeEnumToFile)
  }
}
