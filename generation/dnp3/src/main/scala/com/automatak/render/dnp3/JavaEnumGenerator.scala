package com.automatak.render.dnp3

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

    def events = EventTypes.enums.map(m => EnumConfig(m, path))

    def enums : List[EnumConfig] = List(
      CommandStatus(),
      CommandResult(),
      ControlCode(),
      ChannelState(),
      StackState(),
      LogLevel()
    ).map(e => EnumConfig.apply(e, path)) ::: events

    implicit val indent = CppIndentation()

    def writeEnumToFile(cfg: EnumConfig): Unit = {
        val license = commented(LicenseHeader())
        val enum = EnumModelRenderer.render(cfg.model)
        val lines = license ++ getPackage ++ space ++ enum
        writeLinesTo(cfg.filePath, lines)
        println("Wrote: " + cfg.filePath)
    }

    enums.foreach(writeEnumToFile)
  }
}
