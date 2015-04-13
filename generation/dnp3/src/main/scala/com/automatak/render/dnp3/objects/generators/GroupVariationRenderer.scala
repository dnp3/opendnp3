package com.automatak.render.dnp3.objects.generators

import com.automatak.render.dnp3.objects._

import com.automatak.render._
import com.automatak.render.cpp._

object GroupVariationHeaderRenderer extends ModelRenderer[GroupVariation]{

  private def idDeclaration(gv: GroupVariation): Iterator[String] = Iterator(
    "static GroupVariationID ID() { return GroupVariationID(" + gv.group + "," + gv.variation + "); }"
  )

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = renderStruct(gv, getHeaderLinesFor(gv))

  private def getHeaderLinesFor(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: FixedSize => FixedSizeGenerator.header(x)
    case _ => Iterator.empty
  }

  private def renderStruct(gv: GroupVariation, lines: Iterator[String] = Iterator.empty)(implicit i: Indentation): Iterator[String] = {
    struct(gv.name) { idDeclaration(gv) ++ lines }
  }
}

object GroupVariationImplRenderer extends ModelRenderer[GroupVariation]{

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: FixedSize => FixedSizeGenerator.implementation(x)
    case _ => Iterator.empty
  }

}
