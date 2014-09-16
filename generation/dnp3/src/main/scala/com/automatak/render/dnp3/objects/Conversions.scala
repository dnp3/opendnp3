package com.automatak.render.dnp3.objects

import com.automatak.render._
import com.automatak.render.cpp._

class ArbitraryConversion(name: String, incHeaders: List[String], cppHeaders: List[String]) extends Conversion {

  def target: String = name

  def includeHeaders: List[String] = incHeaders
  def implHeaders: List[String] = cppHeaders

  def signatures : Iterator[String] = {
    Iterator(
      "typedef " + name + " Target;",
      "static " + name + " ReadTarget(openpal::ReadOnlyBuffer&);",
      "static void WriteTarget(const " + name + "&, openpal::WriteBuffer&);"
    )
  }


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    def func1 = {
      Iterator(name + " " + fs.name + "::ReadTarget(ReadOnlyBuffer& buff)") ++ bracket {
        Iterator("auto gv = "+ fs.name + "::Read(buff);",
          "return " + name + "Factory::From(" + args + ");")
      }
    }

    def func2 = {
      Iterator("void " + fs.name + "::WriteTarget(const " + name + "& value, openpal::WriteBuffer& buff)") ++ bracket {
        Iterator(fs.name+"::Write(Convert" + fs.name + "::Apply(value), buff);")
      }
    }

    func1 ++ space ++ func2
  }

}

object ConversionHeaders {

  val dataTypes = quoted("opendnp3/app/MeasurementTypes.h")
  val crob = quoted("opendnp3/app/ControlRelayOutputBlock.h")
  val ao = quoted("opendnp3/app/AnalogOutput.h")
  val factory = quoted("opendnp3/app/MeasurementFactory.h")
  val serializer = quoted("opendnp3/app/DNP3Serializer.h")
  val conversions = quoted("opendnp3/app/WriteConversions.h")

  val cppIncldues = List(factory, conversions)
}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary", List(serializer, dataTypes), cppIncldues)
object DoubleBitBinaryConversion extends ArbitraryConversion("DoubleBitBinary", List(serializer, dataTypes), cppIncldues)
object AnalogConversion extends ArbitraryConversion("Analog", List(serializer, dataTypes), cppIncldues)
object CounterConversion extends ArbitraryConversion("Counter", List(serializer, dataTypes), cppIncldues)
object FrozenCounterConversion extends ArbitraryConversion("FrozenCounter", List(serializer, dataTypes), cppIncldues)
object BinaryOutputStatusConversion extends ArbitraryConversion("BinaryOutputStatus", List(serializer, dataTypes), cppIncldues)
object AnalogOutputStatusConversion extends ArbitraryConversion("AnalogOutputStatus", List(serializer, dataTypes), cppIncldues)
object CrobConversion extends ArbitraryConversion("ControlRelayOutputBlock", List(serializer, crob), cppIncldues)
object AnalogOutputInt16Conversion extends ArbitraryConversion("AnalogOutputInt16", List(serializer, ao), cppIncldues)
object AnalogOutputInt32Conversion extends ArbitraryConversion("AnalogOutputInt32", List(serializer, ao), cppIncldues)
object AnalogOutputFloat32Conversion extends ArbitraryConversion("AnalogOutputFloat32", List(serializer, ao), cppIncldues)
object AnalogOutputDouble64Conversion extends ArbitraryConversion("AnalogOutputDouble64", List(serializer, ao), cppIncldues)

trait ConversionToBinary {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(BinaryConversion)
}

trait ConversionToDoubleBitBinary {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(DoubleBitBinaryConversion)
}


trait ConversionToAnalog {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogConversion)
}

trait ConversionToCounter {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(CounterConversion)
}

trait ConversionToFrozenCounter {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(FrozenCounterConversion)
}


trait ConversionToBinaryOutputStatus {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(BinaryOutputStatusConversion)
}

trait ConversionToAnalogOutputStatus {
  self : FixedSize =>
  override def conversion: Option[Conversion] = Some(AnalogOutputStatusConversion)
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

