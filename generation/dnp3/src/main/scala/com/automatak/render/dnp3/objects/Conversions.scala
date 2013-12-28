package com.automatak.render.dnp3.objects

import com.automatak.render._
import com.automatak.render.cpp.quoted

class ArbitraryConversion(name: String, incHeaders: List[String], cppHeaders: List[String]) extends Conversion {

  def includeHeaders: List[String] = incHeaders
  def implHeaders: List[String] = cppHeaders

  def signatures : Iterator[String] = {
    Iterator(
      "typedef " + name + " Target;",
      "static " + name + " ReadAndConvert(openpal::ReadOnlyBuffer&);",
      "static void ConvertAndWrite(const " + name + "&, openpal::WriteBuffer&);"
    )
  }


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    def func1 = Iterator(name + " " + fs.name + "::ReadAndConvert(ReadOnlyBuffer& buff)") ++ bracket {
      Iterator("auto gv = Read(buff);",
               "return " + name + "Factory::From(" + args + ");")
    }

    def func2 = Iterator("void " + fs.name + "::ConvertAndWrite(const " + name + "& value, openpal::WriteBuffer& buff)") ++ bracket {
      Iterator("Write(Convert" + fs.name + "::Apply(value), buff);")
    }

    func1 ++ space ++ func2
  }

}

object ConversionHeaders {

  val dataTypes = "<opendnp3/DataTypes.h>"
  val crob = "<opendnp3/ControlRelayOutputBlock.h>"
  val ao = "<opendnp3/AnalogOutput.h>"
  val factory = quoted("MeasurementFactory.h")
  val conversions = quoted("../WriteConversions.h")

  val cppIncldues = List(factory, conversions)
}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary", List(dataTypes), cppIncldues)
object AnalogConversion extends ArbitraryConversion("Analog", List(dataTypes), cppIncldues)
object CounterConversion extends ArbitraryConversion("Counter", List(dataTypes), cppIncldues)
object ControlStatusConversion extends ArbitraryConversion("ControlStatus", List(dataTypes), cppIncldues)
object SetpointStatusConversion extends ArbitraryConversion("SetpointStatus", List(dataTypes), cppIncldues)
object CrobConversion extends ArbitraryConversion("ControlRelayOutputBlock", List(crob), cppIncldues)
object AnalogOutputInt16Conversion extends ArbitraryConversion("AnalogOutputInt16", List(ao), cppIncldues)
object AnalogOutputInt32Conversion extends ArbitraryConversion("AnalogOutputInt32", List(ao), cppIncldues)
object AnalogOutputFloat32Conversion extends ArbitraryConversion("AnalogOutputFloat32", List(ao), cppIncldues)
object AnalogOutputDouble64Conversion extends ArbitraryConversion("AnalogOutputDouble64", List(ao), cppIncldues)

trait ConversionToBinary {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(BinaryConversion)
}

trait ConversionToAnalog {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogConversion)
}

trait ConversionToCounter {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(CounterConversion)
}

trait ConversionToControlStatus {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(ControlStatusConversion)
}

trait ConversionToSetpointStatus {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(SetpointStatusConversion)
}

trait ConversionToCROB {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(CrobConversion)
}

trait ConversionToAnalogOutputInt16 {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogOutputInt16Conversion)
}

trait ConversionToAnalogOutputInt32 {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogOutputInt32Conversion)
}

trait ConversionToAnalogOutputFloat32 {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogOutputFloat32Conversion)
}

trait ConversionToAnalogOutputDouble64 {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogOutputDouble64Conversion)
}

