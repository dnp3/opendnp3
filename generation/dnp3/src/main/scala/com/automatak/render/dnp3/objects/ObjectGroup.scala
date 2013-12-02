package com.automatak.render.dnp3.objects

import com.automatak.render.dnp3.objects.groups._

object ObjectGroup {

 val all = List(
    Group1,
    Group2,
    Group3,
    Group4,
    Group10,
    Group11,
    Group12,
    Group20,
    Group21,
    Group22,
    Group23,
    Group30,
    Group32,
    Group40,
    Group41,
    Group42,
    Group50,
    Group51,
    Group60
  )

}

trait ObjectGroup {
  def objects: List[GroupVariation]
  def group: Byte
}
