package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp._

object CppEnumGenerator {

  def apply(directory: Path): Unit = {

    case class EnumConfig(model: EnumModel, conversions: Boolean, stringConv: Boolean) {

      def headerPath(dir: Path) = dir.resolve(headerName)
      def implPath(dir: Path) = dir.resolve(implName)

      def headerName = model.name + ".h"
      def implName = model.name + ".cpp"
    }

    val nsopendnp3 = "opendnp3"

    def includeEnums = List(
      EnumConfig(DoubleBit(), true, true),
      EnumConfig(ScanStatus(), false, true),
      EnumConfig(CommandStatus(), true, true),
      EnumConfig(CommandResult(), false, true),
      EnumConfig(ControlCode(), true, true),
      EnumConfig(ChannelState(), false, true)
    ) ::: EventTypes.enums.map(m => EnumConfig(m, true, false))

    // list of all enumerations that we want to generate
    def sourceEnums = List(FunctionCode(), QualifierCode(), LinkFunction()).map(em => EnumConfig.apply(em, true, true))

    val enums = sourceEnums ::: includeEnums

    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val conversions = if(cfg.conversions) List(EnumToType, EnumFromType) else Nil
      val stringify = if(cfg.stringConv) List(EnumToString) else Nil

      val renders = conversions ::: stringify

      def writeHeader() {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model).toIterator
        def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        def lines = license ++ space ++ includeGuards(cfg.model.name)(string ++ cstdint ++ space ++ namespace(nsopendnp3)(enum ++ space ++ signatures))
        writeTo(cfg.headerPath(directory))(lines)
        println("Wrote: " + cfg.headerPath(directory))
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(cfg.headerName)
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(nsopendnp3)(funcs)
        writeTo(cfg.implPath(directory))(lines)
        println("Wrote: " + cfg.implPath(directory))
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
