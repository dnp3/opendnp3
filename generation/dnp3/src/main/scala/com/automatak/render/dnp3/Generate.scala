package com.automatak.render.dnp3

import com.automatak.render.dnp3.enums.{QualifierCode, FunctionCode}

import com.automatak.render.cpp._

import com.automatak.render._
import java.nio.file.{Files, FileSystems}

object Generate {

  val opendnp3ns = "opendnp3"
  val cppInclude = FileSystems.getDefault().getPath("../cpp/opendnp3/include/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

    if(!Files.exists(cppInclude)) Files.createDirectory(cppInclude)

    // list of all enumerations that we want to generate
    val enums = List(
      FunctionCode(),
      QualifierCode()
    )

    val indent = CppIndentation()

    val emr = EnumModelRenderer(indent)

    def writeLinesToFile(model: EnumModel): Unit = {
      val path = cppInclude.resolve(model.name + ".h")
      val header = commented(LicenseHeader.lines).toIterator
      val enum = emr.render(model).toIterator
      val lines = header ++ space ++ includeGuards(model.name)(cstdint ++ space ++ namespace(opendnp3ns)(enum))

      //lines.foreach(println)

      writeLinesTo(path, lines)
    }

    enums.foreach { e =>
      writeLinesToFile(e)
      println("Created enum: " + e.name)
    }

  }



}
