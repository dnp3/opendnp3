package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.dnp3.enums.generators.{JavaEnumGenerator, CSharpEnumGenerator, CppEnumGenerator}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val csharpPath = FileSystems.getDefault.getPath("../dotnet/bindings/CLRInterface/gen")
  val javaPath = FileSystems.getDefault.getPath("../java/api/src/main/java/com/automatak/dnp3")

  val opendnp3Path = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

    // generate all enumerations

    CppEnumGenerator(opendnp3Path)
    CSharpEnumGenerator("Automatak.DNP3.Interface", csharpPath)
    JavaEnumGenerator("com.automatak.dnp3", javaPath)

    val objectsPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/objects")
    GroupVariationFileGenerator(objectsPath)
  }

}
