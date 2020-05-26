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
import com.automatak.render.dnp3.objects._

object FixedSizeGenerator {

  def header(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def getFieldString(x: FixedSizeField): String = x.cppType + " " + x.name + ";"

    def typedefs(x: FixedSizeField): Iterator[String] = {
      if(x.name == "value") Iterator("typedef %s ValueType;".format(x.cppType))
      else Iterator.empty
    }

    def defaultConstructor: Iterator[String] = Iterator("%s();".format(x.name))

    def members: Iterator[String] =  x.fields.map(f => typedefs(f)).iterator.flatten ++ x.fields.map(f => getFieldString(f)).iterator

    def sizeSignature: Iterator[String] = Iterator("static size_t Size() { return %d; }".format(x.size))

    def readSignature: Iterator[String] = Iterator("static bool Read(ser4cpp::rseq_t&, %s&);".format(x.name))

    def writeSignature: Iterator[String] = Iterator("static bool Write(const %s&, ser4cpp::wseq_t&);".format(x.name))

    space ++
    defaultConstructor ++
    space ++
    sizeSignature ++
    readSignature ++
    writeSignature ++
    space ++
    members

  }

  def implementation(x: FixedSize)(implicit i: Indentation): Iterator[String] = {

    def readSignature: Iterator[String] = Iterator("bool %s::Read(rseq_t& buffer, %s& output)".format(x.name, x.name))

    def writeSignature: Iterator[String] = Iterator("bool %s::Write(const %s& arg, ser4cpp::wseq_t& buffer)".format(x.name, x.name))

    def defaultConstructorSignature: Iterator[String] = Iterator("%s::%s() : %s".format(x.name, x.name, defaultParams), "{}")

    def defaultParams: String = {
      x.fields.map(f => "%s(%s)".format(f.name, f.defaultValue)).mkString(", ")
    }

    def fieldParams(name: String) : String = {
      x.fields.map(f => f.name).map(s => "%s.%s".format(name,s)).mkString(", ")
    }

    def readFunction: Iterator[String] = readSignature ++ bracket {
      FixedSizeHelpers.fixedReads(x.fields, true, "buffer", "output.")
    }

    def writeFunction: Iterator[String] = writeSignature ++ bracket {
      FixedSizeHelpers.fixedWrites(x.fields, true, "buffer", "arg.")
    }

    def defaultConstructor: Iterator[String] = defaultConstructorSignature

    defaultConstructor ++ space ++
    readFunction ++ space ++
    writeFunction
  }
}
