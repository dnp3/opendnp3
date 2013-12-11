package com.automatak.render.dnp3.objects

import com.automatak.render._

class ArbitraryConversion(name: String, header: String) extends Conversion {

  def headers: List[String] = List(header)

  def signatures : Iterator[String] =
    Iterator("typedef " + name + " Target;", "static " + name + " Convert(openpal::ReadOnlyBuffer&);")


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    Iterator(name + " " + fs.name + "::Convert(ReadOnlyBuffer& buff)") ++ bracket {
      Iterator("auto gv = Read(buff);",
               "return " + name + "::From(" + args + ");")
    }
  }

}

object ConversionHeaders {

  val dataTypes = "<opendnp3/DataTypes.h>"
  val crob = "<opendnp3/ControlRelayOutputBlock.h>"

}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary", dataTypes)
object AnalogConversion extends ArbitraryConversion("Analog", dataTypes)
object CounterConversion extends ArbitraryConversion("Counter", dataTypes)
object ControlStatusConversion extends ArbitraryConversion("ControlStatus", dataTypes)
object SetpointStatusConversion extends ArbitraryConversion("SetpointStatus", dataTypes)
object CrobConversion extends ArbitraryConversion("ControlRelayOutputBlock", crob)

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