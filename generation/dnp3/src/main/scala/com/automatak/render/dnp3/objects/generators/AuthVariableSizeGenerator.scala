/**
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements.
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects._

object AuthVariableSizeGenerator {

  def bailoutIf(condition: String)(implicit i: Indentation) : Iterator[String] = {
    Iterator("if(%s)".format(condition)) ++ bracket(Iterator("return false;")) ++ space
  }

  def fields(x: AuthVariableSize): List[Field] = x.fixedFields ::: x.lengthFields ::: x.remainder.toList

  def header(x: AuthVariableSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldString(x: FixedSizeField): String = "%s %s;".format(x.cppType, x.name)

    def getVariableFieldString(name: String): String = "ser4cpp::rseq_t %s;".format(name)

    def members: Iterator[String] =  {
      x.fixedFields.map(f => getFieldString(f)).iterator ++
      x.lengthFields.map(f => getVariableFieldString(f.name)) ++
      x.remainder.map(f => getVariableFieldString(f.name))
    }

    def defaultConstructor = Iterator("%s();".format(x.name))

    def Id = Iterator("virtual GroupVariationID InstanceID() const override final { return ID(); }")

    def primaryConstructor(implicit indent: Indentation) : Iterator[String] = {

      val all = fields(x)
      val count = all.count(_ => true)
      val last = all.last
      val front = all.take(count-1)

      def firstArgs : Iterator[String] = front.map(f => "%s %s,".format(f.cppArgument, f.name)).iterator
      def lastArg : Iterator[String] = Iterator("%s %s".format(last.cppArgument, last.name))

      def constructor = if (count == 1) "explicit %s".format(x.name) else x.name

      Iterator("%s(".format(constructor)) ++ indent {
        firstArgs ++ lastArg
      } ++ Iterator(");")
    }

    def sizeSignature: Iterator[String] = Iterator("virtual size_t Size() const override final;")

    def readSignature: Iterator[String] = Iterator("virtual bool Read(const ser4cpp::rseq_t&) override final;")

    def writeSignature: Iterator[String] = Iterator("virtual bool Write(ser4cpp::wseq_t&) const override final;")

    space ++
    Id ++
    space ++
    defaultConstructor ++
    space ++
    primaryConstructor ++
    space ++
    sizeSignature ++
    readSignature ++
    writeSignature ++
    space ++
    Iterator("static const size_t MIN_SIZE = %s;".format(x.minimumSize)) ++
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

      def firstArgs : Iterator[String] = front.map(f => "%s %s_,".format(f.cppArgument, f.name)).iterator
      def lastArg : Iterator[String] = Iterator("%s %s_".format(last.cppArgument, last.name))

      def firstDefs : Iterator[String] = front.map(f => "%s(%s_),".format(f.name, f.name)).iterator
      def lastDefs : Iterator[String] = Iterator("%s(%s_)".format(last.name, last.name))

      Iterator("%s::%s(".format(x.name, x.name)) ++ indent {
        firstArgs ++ lastArg
      } ++ Iterator(") : ") ++ indent {
        firstDefs ++ lastDefs
      } ++
      Iterator("{}")
    }

    def readSignature: Iterator[String] = Iterator("bool %s::Read(const rseq_t& buffer)".format(x.name))

    def writeSignature: Iterator[String] = Iterator("bool %s::Write(ser4cpp::wseq_t& buffer) const".format(x.name))

    def fieldParams(name: String) : String = {
      x.fixedFields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def variableFields: List[VariableField] = x.lengthFields ::: x.remainder.toList

    def variableFieldSizeSumation: String = variableFields.map(f => "%s.length()".format(f.name)).mkString(" + ")

    def sizeFunction: Iterator[String] = Iterator("size_t %s::Size() const".format(x.name)) ++ bracket {
      Iterator("return MIN_SIZE + %s;".format(variableFieldSizeSumation))
    }

    def readFunction: Iterator[String] = {

      def minSizeBailout = bailoutIf("buffer.length() < %s::MIN_SIZE".format(x.name))

      def copy = Iterator("rseq_t copy(buffer); //mutable copy for parsing")

      def prefixedRead(x : List[VariableField]) : Iterator[String] =  {
        bailoutIf("!PrefixFields::Read(copy, %s)".format(x.map(_.name).mkString(", ")))
      }

      def prefixedReads : Iterator[String] = if(x.lengthFields.isEmpty) Iterator.empty else prefixedRead(x.lengthFields)

      def remainderRead: Iterator[String] = x.remainder match {
        case Some(x) => Iterator("this->%s = copy; // whatever is left over".format(x.name))
        case None => {
          comment("object does not have a remainder field so it should be fully consumed") ++
          comment("The header length disagrees with object encoding so abort") ++
          bailoutIf("copy.is_not_empty()")
        }
      }

      def fullRead = readSignature ++ bracket {
        minSizeBailout ++
        copy ++
        space ++
        FixedSizeHelpers.fixedReads(x.fixedFields, false, "copy", "this->") ++
        prefixedReads ++
        remainderRead ++
        Iterator("return true;")
      }

      def simpleRead(remainder: VariableField) = readSignature ++ bracket {
        Iterator("this->%s = buffer; // the object is just the remainder field".format(remainder.name)) ++
        Iterator("return true;")
      }

      x match {
        case r : RemainderOnly => simpleRead(r.remainderValue)
        case _ => fullRead
      }

    }

    def writeFunction: Iterator[String] = {

      def minSizeBailout = "this->Size() > buffer.length()"

      def uint16Bailouts : Iterator[String] = if(x.lengthFields.isEmpty) Iterator.empty
      else
      {
        comment("All of the fields that have a uint16_t length must have the proper size") ++
        bailoutIf("!PrefixFields::LengthFitsInUInt16(%s)".format(x.lengthFields.map(f => f.name).mkString(", ")))
      }


      def bailoutClauses : Iterator[String] = bailoutIf(minSizeBailout) ++ uint16Bailouts

      def prefixedWrites : Iterator[String] = if(x.lengthFields.isEmpty) Iterator.empty else {
        val fields = x.lengthFields.map(f => f.name).mkString(", ")
        bailoutIf("!PrefixFields::Write(buffer, %s)".format(fields))
      }

      def remainderWrite: Iterator[String] = x.remainder match {
        case Some(x) => Iterator("buffer.copy_from(%s);".format(x.name))
        case None => Iterator.empty
      }

      writeSignature ++ bracket {
          bailoutClauses ++
          FixedSizeHelpers.fixedWrites(x.fixedFields, false, "buffer", "this->") ++
          prefixedWrites ++
          remainderWrite ++
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
