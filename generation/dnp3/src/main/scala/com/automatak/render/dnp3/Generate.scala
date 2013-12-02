package com.automatak.render.dnp3

import java.nio.file.FileSystems

object Generate {

  val csharpInterfacePath = FileSystems.getDefault.getPath("../clr/DNP3CLRInterface/gen")

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
    CppEnumGenerator(opendnp3Paths, openpalPaths)

    CSharpEnumGenerator(List("DNP3","Interface"), csharpInterfacePath)

  }

}
