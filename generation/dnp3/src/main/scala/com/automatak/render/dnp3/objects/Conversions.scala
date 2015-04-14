package com.automatak.render.dnp3.objects

import com.automatak.render._
import com.automatak.render.cpp._

class ArbitraryConversion(name: String) extends Conversion {

  def target: String = name

  def signatures : Iterator[String] = {
    Iterator(
      "typedef %s Target;".format(name),
      "static bool ReadTarget(openpal::ReadBufferView&, %s&);".format(name),
      "static bool WriteTarget(const %s&, openpal::WriteBufferView&);".format(name)
    )
  }


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    def readFunc = {
      val args =  fs.fields.map(f => "value." + f.name).mkString(", ")
      Iterator("bool %s::ReadTarget(ReadBufferView& buff, %s& output)".format(fs.name, name)) ++ bracket {
        Iterator("%s value;".format(fs.name)) ++
        Iterator("if(Read(buff, value))") ++ bracket {
          Iterator("output = %sFactory::From(%s);".format(name, args)) ++
          Iterator("return true;")
        } ++
        Iterator("else") ++ bracket {
          Iterator("return false;")
        }
      }
    }

    def writeFunc = {
      Iterator("bool " + fs.name + "::WriteTarget(const " + name + "& value, openpal::WriteBufferView& buff)") ++ bracket {
        Iterator("return %s::Write(Convert%s::Apply(value), buff);".format(fs.name, fs.name))
      }
    }

    readFunc ++ space ++ writeFunc
  }

}

object ConversionHeaders {

  val dataTypes = quoted("opendnp3/app/MeasurementTypes.h")
  val timeAndInterval = quoted("opendnp3/app/TimeAndInterval.h")
  val crob = quoted("opendnp3/app/ControlRelayOutputBlock.h")
  val ao = quoted("opendnp3/app/AnalogOutput.h")
  val binaryCommandEvent = quoted("opendnp3/app/BinaryCommandEvent.h")
  val analogCommandEvent = quoted("opendnp3/app/AnalogCommandEvent.h")
  val factory = quoted("opendnp3/app/MeasurementFactory.h")
  val serializer = quoted("opendnp3/app/DNP3Serializer.h")
  val conversions = quoted("opendnp3/app/WriteConversions.h")

  val cppIncludes = List(factory, conversions)
}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary")
object DoubleBitBinaryConversion extends ArbitraryConversion("DoubleBitBinary")
object AnalogConversion extends ArbitraryConversion("Analog")
object CounterConversion extends ArbitraryConversion("Counter")
object FrozenCounterConversion extends ArbitraryConversion("FrozenCounter")
object BinaryOutputStatusConversion extends ArbitraryConversion("BinaryOutputStatus")
object AnalogOutputStatusConversion extends ArbitraryConversion("AnalogOutputStatus")
object CrobConversion extends ArbitraryConversion("ControlRelayOutputBlock")
object AnalogOutputInt16Conversion extends ArbitraryConversion("AnalogOutputInt16")
object AnalogOutputInt32Conversion extends ArbitraryConversion("AnalogOutputInt32")
object AnalogOutputFloat32Conversion extends ArbitraryConversion("AnalogOutputFloat32")
object AnalogOutputDouble64Conversion extends ArbitraryConversion("AnalogOutputDouble64")
object TimeAndIntervalConversion extends ArbitraryConversion("TimeAndInterval")
object BinaryCommandEventConversion extends ArbitraryConversion("BinaryCommandEvent")
object AnalogCommandEventConversion extends ArbitraryConversion("AnalogCommandEvent")

trait ConversionToBinary extends FixedSize {

  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(BinaryConversion)

}

trait ConversionToDoubleBitBinary extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(DoubleBitBinaryConversion)
}
trait ConversionToAnalog extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogConversion)
}

trait ConversionToCounter extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(CounterConversion)
}

trait ConversionToFrozenCounter extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(FrozenCounterConversion)
}


trait ConversionToBinaryOutputStatus extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(BinaryOutputStatusConversion)
}

trait ConversionToAnalogOutputStatus extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, dataTypes)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogOutputStatusConversion)
}

trait ConversionToCROB extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, crob)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(CrobConversion)
}

trait ConversionToAnalogOutputInt16 extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, ao)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogOutputInt16Conversion)
}

trait ConversionToAnalogOutputInt32 extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, ao)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogOutputInt32Conversion)
}

trait ConversionToAnalogOutputFloat32 extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, ao)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogOutputFloat32Conversion)
}

trait ConversionToAnalogOutputDouble64 extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, ao)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogOutputDouble64Conversion)
}

trait ConversionToTimeAndInterval extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, timeAndInterval)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(TimeAndIntervalConversion)
}

trait ConversionToBinaryCommandEvent extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, binaryCommandEvent)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(BinaryCommandEventConversion)
}

trait ConversionToAnalogCommandEvent extends FixedSize {
  override def headerIncludes : List[String] = super.headerIncludes ++ List(serializer, analogCommandEvent)
  override def implIncludes : List[String] = super.implIncludes ++ cppIncludes
  override def conversion: Option[Conversion] = Some(AnalogCommandEventConversion)
}

