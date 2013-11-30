package com.automatak.render.dnp3.objects

trait ObjectGroup {
  def objects: List[GroupVariation]
  def group: Byte
}
