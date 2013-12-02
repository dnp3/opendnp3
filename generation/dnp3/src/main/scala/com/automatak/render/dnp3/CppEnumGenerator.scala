package com.automatak.render.dnp3

import com.automatak.render._
import java.nio.file.Path
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp._

object CppEnumGenerator {

  case class DualPath(include: Path, source: Path)



  def apply(opendnp3: DualPath, openpal: DualPath): Unit = {

    case class EnumConfig(model: EnumModel, ns: String, paths: DualPath, conversions: Boolean = true, localInclude: Boolean = true) {
      def headerName = model.name + ".h"
      def implName = model.name + ".cpp"
      def headerPath = paths.include.resolve(headerName)
      def implPath = paths.source.resolve(implName)
    }

    def includeEnums = List(
      EnumConfig(CommandStatus(), "opendnp3", opendnp3, true, false),
      EnumConfig(CommandResult(), "opendnp3", opendnp3, false, false),
      EnumConfig(ControlCode(), "opendnp3", opendnp3, true, false),
      EnumConfig(ChannelState(), "opendnp3", opendnp3, false, false),
      EnumConfig(StackState(), "opendnp3", opendnp3, false, false),
      EnumConfig(LogLevel(), "openpal", openpal, true, false)
    ) ::: EventTypes.enums.map(m => EnumConfig(m, "opendnp3", opendnp3, true, false))

    // list of all enumerations that we want to generate
    def sourceEnums = List(FunctionCode(), QualifierCode(), LinkFunction()).map { em =>
      EnumConfig.apply(em, "opendnp3", opendnp3.copy(include = opendnp3.source))
    }

    val enums = sourceEnums ::: includeEnums

    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val renders = if(cfg.conversions) List(EnumToString, EnumToType, EnumFromType) else List(EnumToString)

      def writeHeader() {
        val license = commented(LicenseHeader())
        val enum = EnumModelRenderer.render(cfg.model).toIterator
        val signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        val lines = license ++ space ++ includeGuards(cfg.model.name)(string ++ cstdint ++ space ++ namespace(cfg.ns)(enum ++ space ++ signatures))
        writeLinesTo(cfg.headerPath, lines)
        println("Wrote: " + cfg.headerPath)
      }

      def writeImpl() {
        val license = commented(LicenseHeader())
        val funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        val inc = if(cfg.localInclude) quoted(cfg.headerName) else bracketed(List(cfg.ns,"/gen/",cfg.headerName).mkString)
        val lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(cfg.ns)(funcs)
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
