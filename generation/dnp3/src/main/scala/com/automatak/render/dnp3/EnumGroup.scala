package com.automatak.render.dnp3

import java.nio.file.Path

import com.automatak.render.EnumModel

case class EnumConfig(model: EnumModel, conversions: Boolean, stringConv: Boolean) {

  def headerPath(dir: Path) = dir.resolve(headerName)
  def implPath(dir: Path) = dir.resolve(implName)

  def headerName = model.name + ".h"
  def implName = model.name + ".cpp"

}

trait EnumGroup {

  def namespace: String

  def enums : List[EnumConfig]
}
