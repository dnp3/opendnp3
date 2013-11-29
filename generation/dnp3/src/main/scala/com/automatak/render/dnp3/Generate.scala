package com.automatak.render.dnp3

import com.automatak.render.dnp3.enums.{LinkFunction, QualifierCode, FunctionCode}

import com.automatak.render.cpp._

import com.automatak.render._
import java.nio.file.{Path, Files, FileSystems}

object Generate {

  val opendnp3 = "opendnp3"
  val basepath = FileSystems.getDefault().getPath("../cpp").resolve(opendnp3)
  val cppInclude = basepath.resolve("include/opendnp3/gen/")
  val cppSource = basepath.resolve("src/opendnp3/gen/")

  def main(args: Array[String]): Unit = {

    case class EnumConfig(model: EnumModel, headerDest: Path, implDest: Path) {
      def headerName = model.name + ".h"
      def implName = model.name + ".cpp"
      def headerPath = headerDest.resolve(headerName)
      def implPath = implDest.resolve(implName)
    }

    val sourceEnums = List(FunctionCode(), QualifierCode(), LinkFunction())

    // list of all enumerations that we want to generate
    val enums = sourceEnums.map(e => EnumConfig(e, cppSource, cppSource))

    val indent = CppIndentation()

    val emr = new EnumModelRenderer(indent)

    val conversionDecls = List(EnumToString.HeaderRender(indent), EnumToType.HeaderRender(indent), EnumFromType.HeaderRender(indent))
    val conversionImpls = List(EnumToString.ImplRender(indent), EnumToType.ImplRender(indent), EnumFromType.ImplRender(indent))

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      def writeHeader() {
        val license = commented(LicenseHeader.lines).toIterator
        val enum = emr.render(cfg.model).toIterator
        val signatures = conversionDecls.map(c => c.render(cfg.model)).flatten.toIterator
        val lines = license ++ space ++ includeGuards(cfg.model.name)(string ++ cstdint ++ space ++ namespace(opendnp3)(enum ++ space ++ signatures))
        writeLinesTo(cfg.headerPath, lines)
        println("Wrote: " + cfg.headerPath)
      }

      def writeImpl() {
        val license = commented(LicenseHeader.lines).toIterator
        val funcs = conversionImpls.map(x => x.render(cfg.model)).flatten.toIterator
        val lines = license ++ space ++ Iterator.apply(include(quotes(cfg.headerName))) ++ space ++ namespace(opendnp3)(funcs)
        writeLinesTo(cfg.implPath, lines)
        println("Wrote: " + cfg.implPath)
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)

    }

  }



}
