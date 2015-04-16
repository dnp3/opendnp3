package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects._

object AuthVariableSizeGenerator {

  case class FieldInfo(name: String, typ: String)

  def bailoutIf(condition: String)(implicit i: Indentation) : Iterator[String] = Iterator("if(%s)".format(condition)) ++ bracket(Iterator("return false;"))

  def fields(x: AuthVariableSize): List[Field] = x.fixedFields ::: x.lengthFields ::: x.remainder.toList

  def header(x: AuthVariableSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldString(x: FixedSizeField): String = "%s %s;".format(x.cppType, x.name)

    def getVariableFieldString(name: String): String = "openpal::ReadBufferView %s;".format(name)

    def members: Iterator[String] =  {
      x.fixedFields.map(f => getFieldString(f)).iterator ++
      x.lengthFields.map(f => getVariableFieldString(f.name)) ++
      x.remainder.map(f => getVariableFieldString(f.name))
    }

    def defaultConstructor = Iterator("%s();".format(x.name))

    def primaryConstructor(implicit indent: Indentation) : Iterator[String] = {

      val all = fields(x)
      val count = all.count(_ => true)
      val last = all.last
      val front = all.take(count-1)

      def firstArgs : Iterator[String] = front.map(f => "%s %s,".format(f.cppArgument, f.name)).toIterator
      def lastArg : Iterator[String] = Iterator("%s %s".format(last.cppArgument, last.name))

      Iterator("%s(".format(x.name)) ++ indent {
        firstArgs ++ lastArg
      } ++ Iterator(");")
    }

    def sizeSignature: Iterator[String] = Iterator("virtual uint32_t Size() const override final;")

    def readSignature: Iterator[String] = Iterator("virtual bool Read(const openpal::ReadBufferView&) override final;")

    def writeSignature: Iterator[String] = Iterator("virtual bool Write(openpal::WriteBufferView&) const override final;")

    space ++
    defaultConstructor ++
    space ++
    primaryConstructor ++
    space ++
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

    def primaryConstructor(implicit indent: Indentation) : Iterator[String] = {

      val all = fields(x)
      val count = all.count(_ => true)
      val last = all.last
      val front = all.take(count-1)

      def firstArgs : Iterator[String] = front.map(f => "%s %s_,".format(f.cppArgument, f.name)).toIterator
      def lastArg : Iterator[String] = Iterator("%s %s_".format(last.cppArgument, last.name))

      def firstDefs : Iterator[String] = front.map(f => "%s(%s_),".format(f.name, f.name)).toIterator
      def lastDefs : Iterator[String] = Iterator("%s(%s_)".format(last.name, last.name))

      Iterator("%s::%s(".format(x.name, x.name)) ++ indent {
        firstArgs ++ lastArg
      } ++ Iterator(") : ") ++ indent {
        firstDefs ++ lastDefs
      } ++
      Iterator("{}")
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

    def readFunction: Iterator[String] = {

      def minSizeBailout = bailoutIf("buffer.Size() < %s::MIN_SIZE".format(x.name))

      def copy = Iterator("ReadBufferView copy(buffer); //mutable copy for parsing")

      def fixedReads : Iterator[String] = {

        def toNumericReadOp(fs: FixedSizeField) : String = {
          "this->%s = %s::ReadBuffer(copy);".format(fs.name, FixedSizeHelpers.getCppFieldTypeParser(fs.typ))
        }

        def toEnumReadOp(fs: FixedSizeField, e: EnumFieldType) : String = {
          "this->%s = %sFromType(UInt8::ReadBuffer(copy));".format(fs.name, e.model.name)
        }


        def toReadOp(fs: FixedSizeField) : String = fs.typ match {
          case x : EnumFieldType => toEnumReadOp(fs, x)
          case _ => toNumericReadOp(fs)
        }

        if(x.fixedFields.isEmpty) Iterator.empty else x.fixedFields.map(toReadOp).iterator ++ space
      }

      def prefixedRead(x : VariableField) : Iterator[String] =  {
        bailoutIf("!IVariableLength::ReadUInt16PrefixedField(copy, this->%s)".format(x.name)) ++ space
      }

      def prefixedReads : Iterator[String] = if(x.lengthFields.isEmpty) Iterator.empty else {
        x.lengthFields.map(prefixedRead).flatten.toIterator
      }

      def remainderRead: Iterator[String] = x.remainder match {
        case Some(x) => Iterator("this->%s = copy; // whatever is left over".format(x.name)) ++ space
        case None => Iterator.empty
      }

      readSignature ++ bracket {
        minSizeBailout ++
        space ++
        copy ++
        space ++
        fixedReads ++
        prefixedReads ++
        remainderRead ++
        Iterator("return true;")
      }
    }

    def writeFunction: Iterator[String] = {

      def vsizeBailout(x: VariableField) : String = "%s.Size() > openpal::MaxValue<uint16_t>()".format(x.name)

      def minSizeBailout = "this->Size() > buffer.Size()"

      def bailouts : List[String] = minSizeBailout :: x.lengthFields.map(vsizeBailout)

      def bailoutClauses : Iterator[String] = bailouts.map { b =>
        Iterator("if(%s)".format(b)) ++ bracket(Iterator("return false;")) ++
        space
      }.toIterator.flatten

      def fixedWrites : Iterator[String] = {

        def toNumericWriteOp(fs: FixedSizeField) : String = {
          "%s::WriteBuffer(buffer, this->%s);".format(FixedSizeHelpers.getCppFieldTypeParser(fs.typ), fs.name)
        }

        def toEnumWriteOp(fs: FixedSizeField, e: EnumFieldType) : String = {
          "UInt8::WriteBuffer(buffer, %sToType(this->%s));".format(e.model.name, fs.name)
        }

        def toWriteOp(fs: FixedSizeField) : String = fs.typ match {
          case x : EnumFieldType => toEnumWriteOp(fs, x)
          case _ => toNumericWriteOp(fs)
        }

        if(x.fixedFields.isEmpty) Iterator.empty else x.fixedFields.map(toWriteOp).iterator ++ space
      }

      def prefixedWrite(x : VariableField) : Iterator[String] = Iterator(
        "UInt16::WriteBuffer(buffer, static_cast<uint16_t>(%s.Size()));".format(x.name),
        "%s.CopyTo(buffer);".format(x.name)
      )

      def prefixedWrites : Iterator[String] = if(x.lengthFields.isEmpty) Iterator.empty else {
        x.lengthFields.map(prefixedWrite).flatten.toIterator ++ space
      }

      def remainderWrite: Iterator[String] = x.remainder match {
        case Some(x) => Iterator("%s.CopyTo(buffer);".format(x.name))
        case None => Iterator.empty
      }

      writeSignature ++ bracket {
          bailoutClauses ++
          fixedWrites ++
          prefixedWrites ++
          remainderWrite ++
          space ++
          Iterator("return true;")
      }
    }

    defaultConstructor ++
    space ++
    primaryConstructor ++
    space ++
    sizeFunction ++
    space ++
    readFunction ++
    space ++
    writeFunction
  }
}
