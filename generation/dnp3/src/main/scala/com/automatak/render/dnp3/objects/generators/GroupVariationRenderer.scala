package com.automatak.render.dnp3.objects.generators

import com.automatak.render.{EnumModel, Indentation, ModelRenderer}
import com.automatak.render.dnp3.objects._

import com.automatak.render.cpp._


object GroupVariationRenderer extends ModelRenderer[GroupVariation]{

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: AnyVariation => render(x)
    case x: SingleBitfield => render(x)
    case x: DoubleBitfield => render(x)
    case x: FixedSize => render(x)
  }


  private def render(x: AnyVariation)(implicit i: Indentation): Iterator[String] = Iterator.empty

  private def render(x: SingleBitfield)(implicit i: Indentation): Iterator[String] = Iterator.empty

  private def render(x: DoubleBitfield)(implicit i: Indentation): Iterator[String] = Iterator.empty

  private def render(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldTypeString(x: FixedSizeFieldType): String = x match {
      case UInt8Field => "uint8_t"
      case UInt16Field => "uint16_t"
      case UInt32Field => "uint32_t"
      case UInt48Field => "uint64_t"
      case SInt16Field => "int16_t"
      case SInt32Field => "int32_t"
      case Float32Field => "float"
      case Float64Field => "double"
      case EnumField(model: EnumModel) => model.name
    }

    def getFieldString(x: FixedSizeField): String = getFieldTypeString(x.typ) + " " + x.name + ";"

    def members: Iterator[String] = x.fields.map(f => getFieldString(f)).iterator

    struct(x.name) {
        members
    }
  }

}
