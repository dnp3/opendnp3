package com.automatak.render.dnp3.objects.generators

import com.automatak.render.dnp3.objects.GroupVariation

object GroupVariationLines {

  def idDeclaration(gv: GroupVariation): Iterator[String] = Iterator(
    "static GroupVariationID ID() { return GroupVariationID(%s,%s); }".format(gv.group, gv.variation)
  )

}
