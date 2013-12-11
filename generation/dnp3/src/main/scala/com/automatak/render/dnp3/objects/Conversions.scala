package com.automatak.render.dnp3.objects

import com.automatak.render._
import com.automatak.render.cpp.quoted

class ArbitraryConversion(name: String, incHeaders: String, cppHeaders: String) extends Conversion {

  def includeHeaders: List[String] = List(incHeaders)
  def implHeaders: List[String] = List(cppHeaders)

  def signatures : Iterator[String] =
    Iterator("typedef " + name + " Target;",
      "static " + name + " Convert(openpal::ReadOnlyBuffer&);")


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    Iterator(name + " " + fs.name + "::Convert(ReadOnlyBuffer& buff)") ++ bracket {
      Iterator("auto gv = Read(buff);",
               "return " + name + "Factory::From(" + args + ");")
    }
  }

}

object ConversionHeaders {

  val dataTypes = "<opendnp3/DataTypes.h>"
  val crob = "<opendnp3/ControlRelayOutputBlock.h>"
  val factory = quoted("MeasurementFactory.h")

}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary", dataTypes, factory)
object AnalogConversion extends ArbitraryConversion("Analog", dataTypes, factory)
object CounterConversion extends ArbitraryConversion("Counter", dataTypes, factory)
object ControlStatusConversion extends ArbitraryConversion("ControlStatus", dataTypes, factory)
object SetpointStatusConversion extends ArbitraryConversion("SetpointStatus", dataTypes, factory)
object CrobConversion extends ArbitraryConversion("ControlRelayOutputBlock", crob, factory)

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