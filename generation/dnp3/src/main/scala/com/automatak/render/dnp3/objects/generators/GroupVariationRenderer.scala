package com.automatak.render.dnp3.objects.generators

import com.automatak.render.dnp3.objects._

import com.automatak.render._
import com.automatak.render.cpp._

object GroupVariationHeaderRenderer extends ModelRenderer[GroupVariation]{

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

    def readSignature: Iterator[String] = Iterator("static " + x.name + " Read(openpal::ReadOnlyBuffer&);")

    def writeSignature: Iterator[String] = Iterator("static void Write(const " + x.name + "&, openpal::WriteBuffer&);")

    struct(x.name) {
        readSignature ++
          writeSignature ++
          space ++
          members
    }
  }
}

object GroupVariationImplRenderer extends ModelRenderer[GroupVariation]{

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

    def getFieldTypeParser(x: FixedSizeFieldType): String = x match {
      case UInt8Field => "UInt8"
      case UInt16Field => "UInt16"
      case UInt32Field => "UInt32"
      case UInt48Field => "UInt48"
      case SInt16Field => "Int16"
      case SInt32Field => "Int32"
      case Float32Field => "SingleFloat"
      case Float64Field => "DoubleFloat"
      case EnumField(model: EnumModel) => model.name
    }

    def readOperation(x: FixedSizeField): Iterator[String] = x.typ match {
      case EnumField(model) =>
        Iterator("obj."+x.name + " = " + getFieldTypeParser(x.typ) + "FromType(UInt8::Read(buffer));", advance(x.typ.numBytes))
      case _ =>
       Iterator("obj."+x.name + " = " + getFieldTypeParser(x.typ) + "::Read(buffer);", advance(x.typ.numBytes))
    }

    def writeOperation(x: FixedSizeField): Iterator[String] = {
      x.typ match {
        case EnumField(model) =>
          Iterator("UInt8::Write(buffer, " + model.name + "ToType(arg." + x.name + "));", advance(x.typ.numBytes))
        case _ =>
          Iterator(getFieldTypeParser(x.typ) + "::Write(buffer, arg." + x.name + ");", advance(x.typ.numBytes))
      }
    }

    def advance(i: Int): String = "buffer.Advance("+i+");"

    def readSignature: Iterator[String] = Iterator(x.name + " " + x.name + "::" + "Read(ReadOnlyBuffer& buffer)")

    def writeSignature: Iterator[String] = Iterator("void " + x.name + "::Write(const " + x.name + "& arg, openpal::WriteBuffer& buffer)")

    def readFunction: Iterator[String] = readSignature ++ bracket {
      Iterator(x.name + " obj;") ++
        x.fields.iterator.map(readOperation).flatten ++
        Iterator("return obj;")
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {

        x.fields.iterator.map(writeOperation).flatten

    }

    readFunction ++ space ++ writeFunction
  }

}
