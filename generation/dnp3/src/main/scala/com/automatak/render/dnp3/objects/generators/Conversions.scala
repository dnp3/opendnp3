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
import com.automatak.render.dnp3.objects.FixedSize

object ConversionHeaders {

  val timeAndInterval: String = quoted("opendnp3/app/TimeAndInterval.h")
  val securityStat: String = quoted("opendnp3/app/SecurityStat.h")
  val crob: String = quoted("opendnp3/app/ControlRelayOutputBlock.h")
  val ao: String = quoted("opendnp3/app/AnalogOutput.h")
  val binaryCommandEvent: String = quoted("opendnp3/app/BinaryCommandEvent.h")
  val analogCommandEvent: String = quoted("opendnp3/app/AnalogCommandEvent.h")

  val measurementTypeSpecs: String = quoted("app/MeasurementTypeSpecs.h")
  val factory: String = quoted("app/MeasurementFactory.h")
  val serializer: String = quoted("app/DNP3Serializer.h")
  val conversions: String = quoted("app/WriteConversions.h")

  val cppIncludes = List(factory, conversions)
}

import com.automatak.render.dnp3.objects.generators.ConversionHeaders._

trait Conversion extends FixedSize {

  def target : String
  def includeSpecTypedef = false
  def spec : String = target + "Spec"

  def convHeaderIncludes : List[String]
  def convImplIncludes : List[String] = cppIncludes

  // override things from GroupVariation w/ additional material

  override def headerLines(implicit i : Indentation) : Iterator[String] = super.headerLines ++ space ++ convHeaderLines
  override def implLines(implicit i : Indentation): Iterator[String] = super.implLines ++ space ++ convImplLines(this)
  override def headerIncludes : List[String] = super.headerIncludes ++ (serializer :: convHeaderIncludes)
  override def implIncludes : List[String] = super.implIncludes ++ convImplIncludes

  def specTypedef : Iterator[String] = if(includeSpecTypedef) Iterator("typedef %s Spec;".format(spec)) else Iterator.empty


  private def convHeaderLines : Iterator[String] = {
    Iterator("typedef %s Target;".format(target)) ++ specTypedef ++
    Iterator(
      "static bool ReadTarget(ser4cpp::rseq_t&, %s&);".format(target),
      "static bool WriteTarget(const %s&, ser4cpp::wseq_t&);".format(target),
      serializerInstance
    )
  }

  def serializerInstance : String  = {
      val serializerType = "DNP3Serializer<%s>".format(target)
      "static %s Inst() { return %s(ID(), Size(), &ReadTarget, &WriteTarget); }".format(serializerType, serializerType)
  }


  private def convImplLines(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    def readFunc = {
      val args =  fs.fields.map(f => "value." + f.name).mkString(", ")
      Iterator("bool %s::ReadTarget(rseq_t& buff, %s& output)".format(fs.name, target)) ++ bracket {
        Iterator("%s value;".format(fs.name)) ++
        Iterator("if(Read(buff, value))") ++ bracket {
          Iterator("output = %sFactory::From(%s);".format(target, args)) ++
          Iterator("return true;")
        } ++
        Iterator("else") ++ bracket {
          Iterator("return false;")
        }
      }
    }

    def writeFunc = {
      Iterator("bool " + fs.name + "::WriteTarget(const " + target + "& value, ser4cpp::wseq_t& buff)") ++ bracket {
        Iterator("return %s::Write(Convert%s::Apply(value), buff);".format(fs.name, fs.name))
      }
    }

    readFunc ++ space ++ writeFunc
  }

}



trait ConversionToBinary extends Conversion {
  def target = "Binary"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToDoubleBitBinary extends Conversion {
  def target = "DoubleBitBinary"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToAnalog extends Conversion {
  def target = "Analog"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToCounter extends Conversion {
  def target = "Counter"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToFrozenCounter extends Conversion {
  def target = "FrozenCounter"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}


trait ConversionToBinaryOutputStatus extends Conversion {
  def target = "BinaryOutputStatus"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToAnalogOutputStatus extends Conversion {
  def target = "AnalogOutputStatus"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToCROB extends Conversion {
  def target = "ControlRelayOutputBlock"
  def convHeaderIncludes = List(crob)
}

trait ConversionToAnalogOutputInt16 extends Conversion {
  def target = "AnalogOutputInt16"
  def convHeaderIncludes = List(ao)
}

trait ConversionToAnalogOutputInt32 extends Conversion {
  def target = "AnalogOutputInt32"
  def convHeaderIncludes = List(ao)
}

trait ConversionToAnalogOutputFloat32 extends Conversion {
  def target = "AnalogOutputFloat32"
  def convHeaderIncludes = List(ao)
}

trait ConversionToAnalogOutputDouble64 extends Conversion {
  def target = "AnalogOutputDouble64"
  def convHeaderIncludes = List(ao)
}

trait ConversionToTimeAndInterval extends Conversion {
  def target = "TimeAndInterval"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}

trait ConversionToBinaryCommandEvent extends Conversion {
  def target = "BinaryCommandEvent"
  def convHeaderIncludes = List(binaryCommandEvent)
}

trait ConversionToAnalogCommandEvent extends Conversion {
  def target = "AnalogCommandEvent"
  def convHeaderIncludes = List(analogCommandEvent)
}

trait ConversionToSecurityStat extends Conversion {
  def target = "SecurityStat"
  override def includeSpecTypedef = true
  def convHeaderIncludes = List(measurementTypeSpecs)
}