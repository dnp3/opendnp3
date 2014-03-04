package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp._

object CppEnumGenerator {

  case class DualPath(include: Path, source: Path)

  def apply(opendnp3: DualPath, openpal: DualPath): Unit = {

    case class EnumConfig(model: EnumModel, ns: String, paths: DualPath, conversions: Boolean = true) {
      def headerName = model.name + ".h"
      def implName = model.name + ".cpp"
      def headerPath = paths.include.resolve(headerName)
      def implPath = paths.source.resolve(implName)
    }

    def includeEnums = List(
      EnumConfig(ScanStatus(), "opendnp3", opendnp3, false),
      EnumConfig(CommandStatus(), "opendnp3", opendnp3, true),
      EnumConfig(CommandResult(), "opendnp3", opendnp3, false),
      EnumConfig(ControlCode(), "opendnp3", opendnp3, true),
      EnumConfig(ChannelState(), "opendnp3", opendnp3, false),
      EnumConfig(StackState(), "opendnp3", opendnp3, false),
      EnumConfig(LogLevel(), "openpal", openpal, true)
    ) ::: EventTypes.enums.map(m => EnumConfig(m, "opendnp3", opendnp3, true))

    // list of all enumerations that we want to generate
    def sourceEnums = List(FunctionCode(), QualifierCode(), LinkFunction()).map { em =>
      EnumConfig.apply(em, "opendnp3", opendnp3.copy(include = opendnp3.source))
    }

    val enums = sourceEnums ::: includeEnums

    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val renders = if(cfg.conversions) List(EnumToString, EnumToType, EnumFromType) else List(EnumToString)

      def writeHeader() {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model).toIterator
        def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        def lines = license ++ space ++ includeGuards(cfg.model.name)(string ++ cstdint ++ space ++ namespace(cfg.ns)(enum ++ space ++ signatures))
        writeTo(cfg.headerPath)(lines)
        println("Wrote: " + cfg.headerPath)
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(cfg.headerName)
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(cfg.ns)(funcs)
        writeTo(cfg.implPath)(lines)
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
