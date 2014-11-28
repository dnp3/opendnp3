package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._

import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp.CppIndentation
import com.automatak.render.csharp._

object CSharpEnumGenerator {

  def apply(ns: String, dir: Path): Unit = {

    case class EnumConfig(model: EnumModel, dest: Path) {
      def fileName = model.name + ".cs"
      def filePath = dest.resolve(fileName)
    }

    def events = EventTypes.enums.map(m => EnumConfig(m, dir))

    def qualityMasks = QualityMasks.enums.map(m => EnumConfig(m, dir))

    def enums : List[EnumConfig] = List(
      AssignClassType(),
      MasterTaskType(),
      StaticTypeBitmask(),
      IntervalUnit(),
      DoubleBit(),
      PointClass(),
      CommandStatus(),
      TaskCompletion(),
      ControlCode(),
      ChannelState(),
      TimeSyncMode(),
      RestartMode(),
      TimestampMode(),
      QualifierCode(),
      GroupVariation()
    ).map(e => EnumConfig.apply(e, dir)) ::: events ::: qualityMasks

    implicit val indent = CppIndentation()

    def writeEnumToFile(cfg: EnumConfig): Unit = {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model)
        def lines = license ++ space ++ namespace(ns)(enum)
        writeTo(cfg.filePath)(lines)
        println("Wrote: " + cfg.filePath)
    }

    enums.foreach(writeEnumToFile)
  }
}
