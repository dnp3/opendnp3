package com.automatak.render.dnp3

import com.automatak.render._
import java.nio.file.Path
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp.CppIndentation
import com.automatak.render.csharp._

object CSharpEnumGenerator {

  def apply(ns: List[String], csharpPath: Path): Unit = {

    case class EnumConfig(model: EnumModel, dest: Path) {
      def fileName = model.name + ".cs"
      def filePath = dest.resolve(fileName)
    }

    def enums = List(
      EnumConfig(CommandStatus(), csharpPath),
      EnumConfig(CommandResult(), csharpPath),
      EnumConfig(ControlCode(), csharpPath)
    )

    implicit val indent = CppIndentation()

    def writeEnumToFile(cfg: EnumConfig): Unit = {
        val license = commented(LicenseHeader())
        val enum = EnumModelRenderer.render(cfg.model)
        val lines = license ++ space ++ space ++ namespace(ns)(enum)
        writeLinesTo(cfg.filePath, lines)
        println("Wrote: " + cfg.filePath)
    }

    enums.foreach(writeEnumToFile)
  }
}
