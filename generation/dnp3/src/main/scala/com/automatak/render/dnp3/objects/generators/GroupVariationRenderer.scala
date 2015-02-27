package com.automatak.render.dnp3.objects.generators

import com.automatak.render.dnp3.objects._

import com.automatak.render._
import com.automatak.render.cpp._

object GroupVariationHeaderRenderer extends ModelRenderer[GroupVariation]{

  private def idDeclaration(gv: GroupVariation): Iterator[String] = Iterator(
    "static GroupVariationID ID() { return GroupVariationID(" + gv.group + "," + gv.variation + "); }"
  )

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: AnyVariation => renderIdOnly(x)
    case x: SingleBitfield => renderIdOnly(x)
    case x: DoubleBitfield => renderIdOnly(x)
    case x: FixedSize => render(x)
  }

  private def renderIdOnly(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = {
    struct(gv.name) {
      idDeclaration(gv)
    }
  }

  private def render(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldTypeString(x: FixedSizeFieldType): String = x match {
      case UInt8Field => "uint8_t"
      case UInt16Field => "uint16_t"
      case UInt32Field => "uint32_t"
      case UInt48Field => "DNPTime"
      case SInt16Field => "int16_t"
      case SInt32Field => "int32_t"
      case Float32Field => "float"
      case Float64Field => "double"
      case EnumField(model: EnumModel) => model.name
    }

    def getFieldString(x: FixedSizeField): String = getFieldTypeString(x.typ) + " " + x.name + ";"

    def typedefs(x: FixedSizeField): Iterator[String] = {
      if(x.name == "value") Iterator("typedef %s ValueType;".format(getFieldTypeString(x.typ)))
      else Iterator.empty
    }

    def members: Iterator[String] =  x.fields.map(f => typedefs(f)).iterator.flatten ++ x.fields.map(f => getFieldString(f)).iterator

    def sizeSignature: Iterator[String] = Iterator("static uint32_t Size() { return %d; }".format(x.size))

    def readSignature: Iterator[String] = Iterator("static bool Read(openpal::ReadBufferView&, %s&);".format(x.name))

    def writeSignature: Iterator[String] = Iterator("static bool Write(const %s&, openpal::WriteBufferView&);".format(x.name))

    def definition : Iterator[String] = struct(x.name) {
      idDeclaration(x) ++
      sizeSignature ++
      readSignature ++
      writeSignature ++
      serializer ++
      members
    }

    def serializer: Iterator[String] = x.conversion match {
      case None => Iterator.empty
      case Some(conv) =>
        val serializerType = "DNP3Serializer<%s>".format(conv.target)
        space ++
        Iterator("static %s Inst() { return %s(ID(), Size(), &ReadTarget, &WriteTarget); }".format(serializerType, serializerType)) ++
        space ++ conv.signatures ++ space
    }

    definition
  }
}

object GroupVariationImplRenderer extends ModelRenderer[GroupVariation]{

  def render(gv: GroupVariation)(implicit i: Indentation): Iterator[String] = gv match {
    case x: FixedSize => renderFixedSize(x)
    case _ => Iterator.empty
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

    def readSignature: Iterator[String] = Iterator("bool " + x.name + "::" + "Read(ReadBufferView& buffer, " + x.name + "& output)")

    def writeSignature: Iterator[String] = Iterator("bool " + x.name + "::Write(const " + x.name + "& arg, openpal::WriteBufferView& buffer)")

    def convertFunction: Iterator[String] = x.conversion match {
      case Some(c) => space ++ c.impl(x) ++ space
      case None => Iterator.empty
    }

    def fieldParams(name: String) : String = {
      x.fields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
        Iterator("return Parse::Many(buffer, " + fieldParams("output") + ");")
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {

        Iterator("return Format::Many(buffer, %s);".format(fieldParams("arg")))

    }

    readFunction ++ space ++
    writeFunction ++ space ++
    convertFunction
  }

}
