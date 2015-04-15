package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects._

object AuthVariableSizeGenerator {

  def header(x: AuthVariableSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldString(x: FixedSizeField): String = "%s %s;".format(FixedSizeHelpers.getCppFieldType(x.typ), x.name)

    def getVariableFieldString(name: String): String = "openpal::ReadBufferView %s;".format(name)

    def members: Iterator[String] =  {
      x.fixedFields.map(f => getFieldString(f)).iterator ++
      x.lengthFields.map(f => getVariableFieldString(f.name)) ++
      x.remainder.map(f => getVariableFieldString(f.name))
    }

    def defaultConstructor = Iterator("%s();".format(x.name))

    def sizeSignature: Iterator[String] = Iterator("virtual uint32_t Size() const override final;")

    def readSignature: Iterator[String] = Iterator("virtual bool Read(const openpal::ReadBufferView&) override final;")

    def writeSignature: Iterator[String] = Iterator("virtual bool Write(openpal::WriteBufferView&) const override final;")

    defaultConstructor ++
    sizeSignature ++
    readSignature ++
    writeSignature ++
    space ++
    Iterator("static const uint32_t MIN_SIZE = %s;".format(x.minimumSize)) ++
    space ++
    comment("member variables") ++
    members

  }

  def implementation(x: AuthVariableSize)(implicit indent: Indentation): Iterator[String] = {

    def defaultConstructor: Iterator[String] = {

      def initializers : Iterator[String] = indent(Iterator(x.fixedFields.map(f => "%s(%s)".format(f.name, f.typ.defaultValue)).mkString(", ")))

      if(x.fixedFields.isEmpty) Iterator("%s::%s()".format(x.name, x.name),"{}")
      else Iterator("%s::%s() : ".format(x.name, x.name)) ++ initializers ++ Iterator("{}")
    }

    def readSignature: Iterator[String] = Iterator("bool %s::Read(const ReadBufferView& buffer)".format(x.name))

    def writeSignature: Iterator[String] = Iterator("bool %s::Write(openpal::WriteBufferView& buffer) const".format(x.name))

    def fieldParams(name: String) : String = {
      x.fixedFields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def variableFields: List[VariableField] = x.lengthFields ::: x.remainder.toList

    def variableFieldSizeSumation: String = variableFields.map(f => "%s.Size()".format(f.name)).mkString(" + ")

    def sizeFunction: Iterator[String] = Iterator("uint32_t %s::Size() const".format(x.name)) ++ bracket {
      Iterator("return MIN_SIZE + %s;".format(variableFieldSizeSumation))
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
      Iterator("return false;")
    }

    def writeFunction: Iterator[String] = {

      def sizeBailout = Iterator("if(buffer.Size() < this->Size())") ++ bracket(Iterator("return false;"))

      def fixedWrites : Iterator[String] = {

        def toNumericWriteOp(fs: FixedSizeField) : String = {
          "openpal::%s::WriteBuffer(buffer, this->%s);".format(FixedSizeHelpers.getCppFieldTypeParser(fs.typ), fs.name)
        }

        def toEnumWriteOp(fs: FixedSizeField, e: EnumFieldType) : String = {
          "openpal::UInt8::WriteBuffer(buffer, %sToType(this->%s));".format(e.model.name, fs.name)
        }


        def toWriteOp(fs: FixedSizeField) : String = fs.typ match {
          case x : EnumFieldType => toEnumWriteOp(fs, x)
          case _ => toNumericWriteOp(fs)
        }

        x.fixedFields.map(toWriteOp).iterator
      }

      def variableWrites: Iterator[String] = variableFields.map(f => "%s.CopyTo(buffer);".format(f.name)).toIterator


      writeSignature ++ bracket {
          sizeBailout ++
          space ++
          fixedWrites ++
          variableWrites ++ // TODO - need to write length fields and enforce UInt16 lengths
          Iterator("return true;")
      }
    }

    defaultConstructor ++
    space ++
    sizeFunction ++
    space ++
    readFunction ++
    space ++
    writeFunction
  }
}
