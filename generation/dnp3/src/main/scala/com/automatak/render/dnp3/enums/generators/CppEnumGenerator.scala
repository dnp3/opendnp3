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

    def fullEnums = List(
      FunctionCode(),
      QualifierCode(),
      LinkFunction(),
      IntervalUnit(),
      ControlCode(),
      GroupVariationEnum(),
      DoubleBit(),
      CommandStatus(),
      HMACType(),
      ChallengeReason(),
      KeyWrapAlgorithm(),
      KeyStatus(),
      AuthErrorCode(),
      CertificateType(),
      KeyChangeMethod()
    ).map(x => EnumConfig(x, true, true))

    def simpleEnums = (List(
      MasterTaskType(),
      AssignClassType(),
      StaticTypeBitmask(),
      PointClass(),
      TimeSyncMode(),
      RestartMode(),
      TimestampMode(),
      EventMode(),
      IndexMode(),
      ConfigAuthMode()
    ) ::: DefaultVariations.enums ::: QualityMasks.enums).map(x => EnumConfig(x, false, false))

    def stringOnlyEnums = List(TaskCompletion(), ChannelState()).map(x => EnumConfig(x, false, true))

    def enums = List(fullEnums, simpleEnums, stringOnlyEnums).flatten

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
