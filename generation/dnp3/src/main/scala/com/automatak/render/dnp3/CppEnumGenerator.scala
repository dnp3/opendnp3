package com.automatak.render.dnp3

import com.automatak.render._
import java.nio.file.Path
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp._

object CppEnumGenerator {

  def apply(ns: String, cppInclude: Path, cppSource: Path): Unit = {

    case class EnumConfig(model: EnumModel, headerDest: Path, implDest: Path, conversions: Boolean = true, localInclude: Boolean = true) {
      def headerName = model.name + ".h"
      def implName = model.name + ".cpp"
      def headerPath = headerDest.resolve(headerName)
      def implPath = implDest.resolve(implName)
    }

    def includeEnums = List(
      EnumConfig(CommandStatus(), cppInclude, cppSource, true, false),
      EnumConfig(CommandResult(), cppInclude, cppSource, false, false),
      EnumConfig(ControlCode(), cppInclude, cppSource, true, false),
      EnumConfig(ChannelState(), cppInclude, cppSource, false, false),
      EnumConfig(StackState(), cppInclude, cppSource, false, false)
    )

    // list of all enumerations that we want to generate
    def sourceEnums = List(FunctionCode(), QualifierCode(), LinkFunction()).map(EnumConfig.apply(_, cppSource, cppSource))

    val enums = sourceEnums ::: includeEnums

    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val renders = if(cfg.conversions) List(EnumToString, EnumToType, EnumFromType) else List(EnumToString)

      def writeHeader() {
        val license = commented(LicenseHeader())
        val enum = EnumModelRenderer.render(cfg.model).toIterator
        val signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        val lines = license ++ space ++ includeGuards(cfg.model.name)(string ++ cstdint ++ space ++ namespace(ns)(enum ++ space ++ signatures))
        writeLinesTo(cfg.headerPath, lines)
        println("Wrote: " + cfg.headerPath)
      }

      def writeImpl() {
        val license = commented(LicenseHeader())
        val funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        val inc = if(cfg.localInclude) quoted(cfg.headerName) else bracketed(List(ns,"/gen/",cfg.headerName).mkString)
        val lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(ns)(funcs)
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
