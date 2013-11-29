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
    val e2s = new EnumToString.HeaderRender(indent)
    val eft = new EnumFromType.HeaderRender(indent)
    val e2t = new EnumToType.HeaderRender(indent)

    val conversions = List(e2s,eft,e2t)

    val impls = List(EnumToString.ImplRender(indent))

    def writeLinesToFile(model: EnumModel): Unit = {
      val pathHeader = cppInclude.resolve(model.name + ".h")
      val pathImpl = cppInclude.resolve(model.name + ".cpp")

      def writeHeader() {
        val license = commented(LicenseHeader.lines).toIterator
        val enum = emr.render(model).toIterator
        val signatures = conversions.map(c => c.render(model)).flatten.toIterator
        val lines = license ++ space ++ includeGuards(model.name)(string ++ cstdint ++ space ++ namespace(opendnp3)(enum ++ space ++ signatures))
        writeLinesTo(pathHeader, lines)
      }

      def writeImpl() {
        val license = commented(LicenseHeader.lines).toIterator
        val funcs = impls.map(x => x.render(model)).flatten.toIterator
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
