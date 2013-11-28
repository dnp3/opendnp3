package com.automatak.dnp3.cpp

import com.automatak.dnp3.{EnumModel, Lines, ModelRenderer}

object EnumerationFileRenderer extends ModelRenderer[EnumModel] {

  def apply(a : EnumModel): Lines = new Lines {

    def foreach[U](f: (String) => U): Unit = {
       EnumModelRenderer(a).foreach(f)
    }

  }

}
