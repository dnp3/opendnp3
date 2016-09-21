package com.automatak.render.dnp3.enums.groups

import com.automatak.render.EnumModel
import com.automatak.render.dnp3.enums._


object JavaEnumGroup {

  def enums : List[EnumModel] = List(

  ) ::: DefaultVariations.enums ::: QualityMasks.enums


}
