package com.automatak.render.dnp3

import java.nio.file.FileSystems

object Generate {

  val basepath = FileSystems.getDefault.getPath("../cpp/opendnp3")
  val cppInclude = basepath.resolve("include/opendnp3/gen/")
  val cppSource = basepath.resolve("src/opendnp3/gen/")

  val csharpInterfacePath = FileSystems.getDefault.getPath("../clr/DNP3CLRInterface/gen")

  def main(args: Array[String]): Unit = {

    // generate all enumerations
    CppEnumGenerator("opendnp3", cppInclude, cppSource)

    CSharpEnumGenerator(List("DNP3","Interface"), csharpInterfacePath)

  }

}
