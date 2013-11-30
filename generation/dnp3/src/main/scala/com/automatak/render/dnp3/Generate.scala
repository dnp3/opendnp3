package com.automatak.render.dnp3

import java.nio.file.FileSystems

object Generate {

  val opendnp3 = "opendnp3"
  val basepath = FileSystems.getDefault().getPath("../cpp").resolve(opendnp3)
  val cppInclude = basepath.resolve("include/opendnp3/gen/")
  val cppSource = basepath.resolve("src/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

    // generate all enumerations
    EnumGenerator(opendnp3, cppSource)

  }

}
