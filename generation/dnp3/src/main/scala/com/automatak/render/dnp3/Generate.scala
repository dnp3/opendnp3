package com.automatak.render.dnp3

import com.automatak.render.dnp3.enums.{QualifierCode, FunctionCode}

import com.automatak.render.cpp.{CppIndentation, EnumModelRenderer}

import com.automatak.render._
import java.nio.file.{Files, FileSystems}

object Generate {

  val cppInclude = FileSystems.getDefault().getPath("../cpp/opendnp3/include/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

     if(!Files.exists(cppInclude)) Files.createDirectory(cppInclude)

    // list of all enumerations that we want to generate
    val enums = List(
      FunctionCode(),
      QualifierCode()
    )

    val emr = EnumModelRenderer(CppIndentation())

    enums.foreach(e => writeEnumFile(emr)(e))
  }

  def writeEnumFile(render: EnumModelRenderer)(model: EnumModel): Unit = {
      val path = cppInclude.resolve(model.name + ".hpp")
      writeLinesTo(path, render.render(model))
  }

}
