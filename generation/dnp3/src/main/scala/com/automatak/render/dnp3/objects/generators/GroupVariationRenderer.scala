package com.automatak.render.dnp3.objects.generators

import com.automatak.render.dnp3.objects._

import com.automatak.render._
import com.automatak.render.cpp._

object GroupVariationHeaderRenderer extends ModelRenderer[GroupVariation]{

  private def idDeclaration: Iterator[String] = Iterator("static const GroupVariationID ID;")

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: AnyVariation => renderIdOnly(x)
    case x: SingleBitfield => renderIdOnly(x)
    case x: DoubleBitfield => renderIdOnly(x)
    case x: FixedSize => render(x)
  }

  private def renderIdOnly(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = {
    struct(gv.name) {
      idDeclaration
    }
  }

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

    def typedefs(x: FixedSizeField): Iterator[String] = {
      if(x.name == "value") Iterator("typedef " + getFieldTypeString(x.typ) + " ValueType;")
      else Iterator.empty
    }

    def members: Iterator[String] =  x.fields.map(f => typedefs(f)).iterator.flatten ++ x.fields.map(f => getFieldString(f)).iterator

    def sizeSignature: Iterator[String] = Iterator("static const uint32_t SIZE = " + x.size + ";")

    def readSignature: Iterator[String] = Iterator("static " + x.name + " Read(openpal::ReadOnlyBuffer&);")

    def writeSignature: Iterator[String] = Iterator("static void Write(const " + x.name + "&, openpal::WriteBuffer&);")

    def definition : Iterator[String] = struct(x.name) {
      idDeclaration ++
      x.conversion.map(c => Iterator("typedef " + c.target + " Target;")).getOrElse(Iterator.empty) ++
      sizeSignature ++
      readSignature ++
      writeSignature ++
      space ++
      members
    }

    def serializer: Iterator[String] = x.conversion match {
      case None => Iterator.empty
      case Some(conv) =>
        val structName = x.name+"Serializer"
        val baseClass = "IDNP3Serializer<"+conv.target+">"
        space ++ struct(structName, Some(baseClass)) {
          space ++ Iterator("static " + baseClass + "& Inst() { return instance; }") ++
          space ++ Iterator("GroupVariationID ID() const { return " + x.name + "::ID; }") ++
          space ++ Iterator("uint32_t Size() const { return " + x.name + "::SIZE; }") ++
          space ++ conv.signatures ++ space ++
          privateSection {
            Iterator("static " + structName + " instance;")
          }
        }
    }

    definition ++ serializer
  }
}

object GroupVariationImplRenderer extends ModelRenderer[GroupVariation]{

  private def idDefintion(gv: GroupVariation): Iterator[String] =
    Iterator("const GroupVariationID  " + gv.name + "::ID(" + gv.group + "," + gv.variation + ");")

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: FixedSize => renderFixedSize(x)
    case _ => renderIdOnly(gv)
  }

  private def renderIdOnly(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = {
    idDefintion(gv)
  }


  private def renderFixedSize(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

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



    def convertFunction: Iterator[String] = x.conversion match {
      case Some(c) => space ++ c.impl(x) ++ space
      case None => Iterator.empty
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
      Iterator(x.name + " obj;") ++
        x.fields.iterator.map(readOperation).flatten ++
        Iterator("return obj;")
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {

        x.fields.iterator.map(writeOperation).flatten

    }

    idDefintion(x) ++ space ++
    readFunction ++ space ++
    writeFunction ++
    convertFunction
  }

}
