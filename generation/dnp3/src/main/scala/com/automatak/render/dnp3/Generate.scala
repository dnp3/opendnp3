package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.dnp3.enums.generators.{CppEnumGenerator}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val dnp3GenPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/gen")
  val dnp3ObjectPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/objects")

  def main(args: Array[String]): Unit = {

    // generate all the required filter items
    CppEnumGenerator(dnp3GenPath)
    GroupVariationFileGenerator(dnp3ObjectPath)
  }

}
