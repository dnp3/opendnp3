package com.automatak.render.dnp3.enums.generators

import _root_.java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp.EnumModelRenderer
import com.automatak.render.dnp3.{EnumGroup, EnumConfig}
import com.automatak.render.dnp3.enums._
import com.automatak.render.cpp._

object CppEnumGenerator {

  def apply(eg: EnumGroup, incDirectory: Path, implDirectory: Path): Unit = {

    implicit val indent = CppIndentation()

    def writeEnumToFiles(cfg: EnumConfig): Unit = {

      val conversions = if(cfg.conversions) List(EnumToType, EnumFromType) else Nil
      val stringify = if(cfg.stringConv) List(EnumToString) else Nil

      val renders = conversions ::: stringify

      def writeHeader() {
        def license = commented(LicenseHeader())
        def enum = EnumModelRenderer.render(cfg.model)
        def signatures = renders.map(c => c.header.render(cfg.model)).flatten.toIterator
        def lines = license ++ space ++ includeGuards(cfg.model.name)(cstdint ++ space ++ namespace(eg.namespace)(enum ++ space ++ signatures))
        writeTo(cfg.headerPath(incDirectory))(lines)
        println("Wrote: " + cfg.headerPath(incDirectory))
      }

      def writeImpl() {
        def license = commented(LicenseHeader())
        def funcs = renders.map(r => r.impl.render(cfg.model)).flatten.toIterator
        def inc = quoted(String.format("opendnp3/gen/%s", cfg.headerName))
        def lines = license ++ space ++ Iterator(include(inc)) ++ space ++ namespace(eg.namespace)(funcs)

        if(cfg.conversions || cfg.stringConv)
        {
          writeTo(cfg.implPath(implDirectory))(lines)
          println("Wrote: " + cfg.implPath(implDirectory))
        }
      }

      writeHeader()
      writeImpl()
    }

    eg.enums.foreach { e =>
      writeEnumToFiles(e)
    }
  }
}
