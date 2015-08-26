package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.dnp3.enums.generators.{CSharpEnumGenerator, CppEnumGenerator}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val dnp3GenPath = FileSystems.getDefault.getPath("../cpp/libs/src/opendnp3/gen")
  val dnp3ObjectPath = FileSystems.getDefault.getPath("../cpp/libs/src/opendnp3/objects")

  val csharpGenPath = FileSystems.getDefault.getPath("../dotnet/bindings/CLRInterface/gen")

  def main(args: Array[String]): Unit = {

    // generate all the required filter items
    CppEnumGenerator(dnp3GenPath)
    CSharpEnumGenerator("Automatak.DNP3.Interface", csharpGenPath)
    GroupVariationFileGenerator(dnp3ObjectPath)
  }

}
