package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.cpp.CppIndentation
import com.automatak.render.dnp3.enums.generators.{JavaEnumGenerator, CSharpEnumGenerator, CppEnumGenerator}
import com.automatak.render.dnp3.filter.{ParseResultGenerator, ParseResultEnum, FilterLookupGenerator, FixedSizeLookup}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val opendnp3GenPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/gen")
  val opendnp3ObjectPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3/objects")
  val opendnp3FilterPath = FileSystems.getDefault.getPath("../cpp/libs/opendnp3/src/opendnp3")

  def main(args: Array[String]): Unit = {

    // generate all the required filter items
    CppEnumGenerator(opendnp3GenPath)
    FilterLookupGenerator(opendnp3GenPath)
    ParseResultGenerator(opendnp3FilterPath)

    // perhaps re-enable later
    // GroupVariationFileGenerator(opendnp3ObjectPath)
  }

}
