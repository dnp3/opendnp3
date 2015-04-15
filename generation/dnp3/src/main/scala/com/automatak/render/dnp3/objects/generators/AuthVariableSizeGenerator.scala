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

    def readSignature: Iterator[String] = Iterator("virtual bool Read(openpal::ReadBufferView&) override final;")

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

    def readSignature: Iterator[String] = Iterator("bool %s::Read(ReadBufferView& buffer, %s& output)".format(x.name, x.name))

    def writeSignature: Iterator[String] = Iterator("bool %s::Write(const %s& arg, openpal::WriteBufferView& buffer)".format(x.name, x.name))

    def fieldParams(name: String) : String = {
      x.fixedFields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def variableFieldSizes: String = (x.lengthFields ::: x.remainder.toList).map(f => "%s.Size()".format(f.name)).mkString(" + ")

    def sizeFunction: Iterator[String] = Iterator("uint32_t %s::Size() const".format(x.name)) ++ bracket {
      Iterator("return MIN_SIZE + %s;".format(variableFieldSizes))
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
      Iterator("return Parse::Many(buffer, %s);".format(fieldParams("output")))
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {
      Iterator("return Format::Many(buffer, %s);".format(fieldParams("arg")))
    }

    defaultConstructor ++
    space ++
    sizeFunction
  }
}
