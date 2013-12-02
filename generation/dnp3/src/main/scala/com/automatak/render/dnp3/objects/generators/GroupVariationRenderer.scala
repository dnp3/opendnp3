package com.automatak.render.dnp3.objects.generators

import com.automatak.render.{Indentation, ModelRenderer}
import com.automatak.render.dnp3.objects.GroupVariation


object GroupVariationRenderer extends ModelRenderer[GroupVariation]{

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = {
    Iterator.empty
  }

}
