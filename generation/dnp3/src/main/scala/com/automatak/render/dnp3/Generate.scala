package com.automatak.render.dnp3

import com.automatak.render.dnp3.enums.{QualifierCode, FunctionCode}

import com.automatak.render.cpp._

import com.automatak.render._
import java.nio.file.{Files, FileSystems}

object Generate {

  val opendnp3 = "opendnp3"
  val cppInclude = FileSystems.getDefault().getPath("../cpp/opendnp3/include/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

    if(!Files.exists(cppInclude)) Files.createDirectory(cppInclude)

    // list of all enumerations that we want to generate
    val enums = List(
      FunctionCode(),
      QualifierCode()
    )

    val indent = CppIndentation()

    val emr = new EnumModelRenderer(indent)

    val conversionDecls = List(EnumToString.HeaderRender(indent), EnumToType.HeaderRender(indent), EnumFromType.HeaderRender(indent))
    val conversionImpls = List(EnumToString.ImplRender(indent), EnumToType.ImplRender(indent), EnumFromType.ImplRender(indent))

    def writeLinesToFile(model: EnumModel): Unit = {
      val pathHeader = cppInclude.resolve(model.name + ".h")
      val pathImpl = cppInclude.resolve(model.name + ".cpp")

      def writeHeader() {
        val license = commented(LicenseHeader.lines).toIterator
        val enum = emr.render(model).toIterator
        val signatures = conversionDecls.map(c => c.render(model)).flatten.toIterator
        val lines = license ++ space ++ includeGuards(model.name)(string ++ cstdint ++ space ++ namespace(opendnp3)(enum ++ space ++ signatures))
        writeLinesTo(pathHeader, lines)
      }

      def writeImpl() {
        val license = commented(LicenseHeader.lines).toIterator
        val funcs = conversionImpls.map(x => x.render(model)).flatten.toIterator
        val lines = license ++ space ++ Iterator.apply(include(quotes(model.name + ".h"))) ++ space ++ namespace(opendnp3)(funcs)
        writeLinesTo(pathImpl, lines)
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeLinesToFile(e)
      println("Created enum: " + e.name)
    }

  }



}
