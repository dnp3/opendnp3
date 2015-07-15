package com.automatak.render.dnp3.objects.generators

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.dnp3.objects.FixedSize

object ConversionHeaders {

  val dataTypes = quoted("opendnp3/app/MeasurementTypes.h")
  val timeAndInterval = quoted("opendnp3/app/TimeAndInterval.h")
  val securityStat = quoted("opendnp3/app/SecurityStat.h")
  val crob = quoted("opendnp3/app/ControlRelayOutputBlock.h")
  val ao = quoted("opendnp3/app/AnalogOutput.h")
  val binaryCommandEvent = quoted("opendnp3/app/BinaryCommandEvent.h")
  val analogCommandEvent = quoted("opendnp3/app/AnalogCommandEvent.h")
  val factory = quoted("opendnp3/app/MeasurementFactory.h")
  val serializer = quoted("opendnp3/app/DNP3Serializer.h")
  val conversions = quoted("opendnp3/app/WriteConversions.h")

  val cppIncludes = List(factory, conversions)
}

import com.automatak.render.dnp3.objects.generators.ConversionHeaders._

trait Conversion extends FixedSize {

  def target : String
  def convHeaderIncludes : List[String]
  def convImplIncludes : List[String] = cppIncludes

  // override things from GroupVariation w/ additional material

  override def headerLines(implicit i : Indentation) : Iterator[String] = super.headerLines ++ space ++ convHeaderLines
  override def implLines(implicit i : Indentation): Iterator[String] = super.implLines ++ space ++ convImplLines(this)
  override def headerIncludes : List[String] = super.headerIncludes ++ (serializer :: convHeaderIncludes)
  override def implIncludes : List[String] = super.implIncludes ++ convImplIncludes


  private def convHeaderLines : Iterator[String] = {
    Iterator(
      "typedef %s Target;".format(target),
      "static bool ReadTarget(openpal::ReadBufferView&, %s&);".format(target),
      "static bool WriteTarget(const %s&, openpal::WriteBufferView&);".format(target),
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
      Iterator("bool %s::ReadTarget(ReadBufferView& buff, %s& output)".format(fs.name, target)) ++ bracket {
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
      Iterator("bool " + fs.name + "::WriteTarget(const " + target + "& value, openpal::WriteBufferView& buff)") ++ bracket {
        Iterator("return %s::Write(Convert%s::Apply(value), buff);".format(fs.name, fs.name))
      }
    }

    readFunc ++ space ++ writeFunc
  }

}



trait ConversionToBinary extends Conversion {
  def target = "Binary"
  def convHeaderIncludes = List(dataTypes)
}

trait ConversionToDoubleBitBinary extends Conversion {
  def target = "DoubleBitBinary"
  def convHeaderIncludes = List(dataTypes)
}

trait ConversionToAnalog extends Conversion {
  def target = "Analog"
  def convHeaderIncludes = List(dataTypes)
}

trait ConversionToCounter extends Conversion {
  def target = "Counter"
  def convHeaderIncludes = List(dataTypes)
}

trait ConversionToFrozenCounter extends Conversion {
  def target = "FrozenCounter"
  def convHeaderIncludes = List(dataTypes)
}


trait ConversionToBinaryOutputStatus extends Conversion {
  def target = "BinaryOutputStatus"
  def convHeaderIncludes = List(dataTypes)
}

trait ConversionToAnalogOutputStatus extends Conversion {
  def target = "AnalogOutputStatus"
  def convHeaderIncludes = List(dataTypes)
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
  def convHeaderIncludes = List(timeAndInterval)
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
  def convHeaderIncludes = List(securityStat)
}