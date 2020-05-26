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
package com.automatak.render.dnp3

import java.nio.file.FileSystems

import com.automatak.render.dnp3.enums.generators.{CSharpEnumGenerator, CppEnumGenerator, JavaEnumGenerator}
import com.automatak.render.dnp3.enums.groups.{CSharpEnumGroup, DNPCppEnumGroup}
import com.automatak.render.dnp3.objects.generators.{AttributeGenerator, GroupVariationFileGenerator}

object Generate {

  private val dnp3GenHeaderPath = FileSystems.getDefault.getPath("../cpp/lib/include/opendnp3/gen")
  private val dnp3GenImplPath = FileSystems.getDefault.getPath("../cpp/lib/src/gen")
  private val csharpGenPath = FileSystems.getDefault.getPath("../dotnet/CLRInterface/src/gen")
  private val dnp3ObjectPath = FileSystems.getDefault.getPath("../cpp/lib/src/gen/objects")
  private val javaGenPath = FileSystems.getDefault.getPath("../java/bindings/src/main/java/com/automatak/dnp3/enums")

  def main(args: Array[String]): Unit = {

    // generate the C++ dnp3 enums
    CppEnumGenerator(DNPCppEnumGroup.enums, "opendnp3", "opendnp3/gen/%s", dnp3GenHeaderPath, dnp3GenImplPath)

    // generate the C++ variation attribute lookups
    AttributeGenerator.writeAttributes("opendnp3", dnp3GenHeaderPath, dnp3GenImplPath)

    // generate the C# enums
    CSharpEnumGenerator(CSharpEnumGroup.enums, "Automatak.DNP3.Interface", csharpGenPath)

    // generate the Java enums
    JavaEnumGenerator(CSharpEnumGroup.enums, "package com.automatak.dnp3.enums;", javaGenPath)

    // generate all the group/variation parsers
    GroupVariationFileGenerator(dnp3ObjectPath)
  }

}
