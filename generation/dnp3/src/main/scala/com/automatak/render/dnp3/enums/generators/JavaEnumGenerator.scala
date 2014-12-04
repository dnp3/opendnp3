package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path
import com.automatak.render._
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp.CppIndentation
import com.automatak.render.java._

object JavaEnumGenerator {

  def apply(ns: String, path: Path): Unit = {

    def getPackage = Iterator(List("package ",ns,";").mkString)

    case class EnumConfig(model: EnumModel, dest: Path) {
      def fileName = model.name + ".java"
      def filePath = dest.resolve(fileName)
    }

    def events = DefaultVariations.enums.map(m => EnumConfig(m, path))

    def enums : List[EnumConfig] = List(
      CommandStatus(),
      TaskCompletion(),
      ControlCode(),
      ChannelState(),
      LogLevel()
    ).map(e => EnumConfig.apply(e, path)) ::: events

    implicit val indent = CppIndentation()

    def writeEnumToFile(cfg: EnumConfig): Unit = {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model)
        def lines = license ++ getPackage ++ space ++ enum
        writeTo(cfg.filePath)(lines)
        println("Wrote: " + cfg.filePath)
    }

    enums.foreach(writeEnumToFile)
  }
}
