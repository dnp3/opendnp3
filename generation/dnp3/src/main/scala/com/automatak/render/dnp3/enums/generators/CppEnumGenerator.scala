package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._
import com.automatak.render.cpp.EnumModelRenderer
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

    def enums = List(
      EnumConfig(FunctionCode(), true, true),
      EnumConfig(QualifierCode(), true, true),
      EnumConfig(LinkFunction(), true, true),
      EnumConfig(IntervalUnit(), true, true),
      EnumConfig(GroupVariationEnum(), true, true),
      EnumConfig(DoubleBit(), true, true),
      EnumConfig(CommandStatus(), true, true),
      EnumConfig(ControlCode(), true, true),
      EnumConfig(QualifierCode(), true, true),
      EnumConfig(HMACType(), true, true),
      EnumConfig(ChallengeReason(), true, true),
      EnumConfig(KeyWrapAlgorithm(), true, true),
      EnumConfig(KeyStatus(), true, true),
      EnumConfig(AuthErrorCode(), true, true),
      EnumConfig(CertificateType(), true, true),
      EnumConfig(ConfigAuthMode(), false, false),
      EnumConfig(GroupVariationType(), false, false),
      EnumConfig(FilterResult(), false, true)
    ) ::: QualityMasks.enums.map(m => EnumConfig(m, false, false))


    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val conversions = if(cfg.conversions) List(EnumToType, EnumFromType) else Nil
      val stringify = if(cfg.stringConv) List(EnumToString) else Nil

      val renders = conversions ::: stringify

      def writeHeader() {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model).toIterator
        def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        def lines = license ++ space ++ includeGuards(cfg.model.name)(cstdint ++ space ++ namespace(nsopendnp3)(enum ++ space ++ signatures))
        writeTo(cfg.headerPath(directory))(lines)
        println("Wrote: " + cfg.headerPath(directory))
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(cfg.headerName)
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(nsopendnp3)(funcs)

        if(cfg.conversions || cfg.stringConv)
        {
          writeTo(cfg.implPath(directory))(lines)
          println("Wrote: " + cfg.implPath(directory))
        }
      }

      writeHeader()
      writeImpl()
    }

    enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
