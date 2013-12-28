package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.dnp3.enums.generators.{JavaEnumGenerator, CSharpEnumGenerator, CppEnumGenerator}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val csharpPath = FileSystems.getDefault.getPath("../clr/DNP3CLRInterface/gen")
  val javaPath = FileSystems.getDefault.getPath("../java/api/src/main/java/com/automatak/dnp3")

  val opendnp3Paths = CppEnumGenerator.DualPath(
    FileSystems.getDefault.getPath("../cpp/opendnp3/include/opendnp3/gen/"),
    FileSystems.getDefault.getPath("../cpp/opendnp3/src/opendnp3/gen/")
  )

  val openpalPaths = CppEnumGenerator.DualPath(
    FileSystems.getDefault.getPath("../cpp/openpal/include/openpal/gen/"),
    FileSystems.getDefault.getPath("../cpp/openpal/src/gen/")
  )

  def main(args: Array[String]): Unit = {

    // generate all enumerations

    /*
    CppEnumGenerator(opendnp3Paths, openpalPaths)
    CSharpEnumGenerator("DNP3.Interface", csharpPath)
    JavaEnumGenerator("com.automatak.dnp3", javaPath)
    */


    val objectsPath = FileSystems.getDefault.getPath("../cpp/opendnp3/src/opendnp3/objects")
    GroupVariationFileGenerator(objectsPath)
  }

}
