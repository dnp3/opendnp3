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
      name + " Read(openpal::ReadOnlyBuffer&) const;",
      "void Write(const " + name + "&, openpal::WriteBuffer&) const;"
    )
  }


  def impl(fs: FixedSize)(implicit indent: Indentation): Iterator[String] = {

    def args : String = fs.fields.map(f => "gv."+f.name).mkString(", ")

    val serializer = fs.name + "Serializer"

    def singleton = Iterator(serializer + " " + serializer + "::mInstance;")

    def func1 = {
      Iterator(name + " " + serializer + "::Read(ReadOnlyBuffer& buff) const") ++ bracket {
        Iterator("auto gv = "+ fs.name + "::Read(buff);",
          "return " + name + "Factory::From(" + args + ");")
      }
    }

    def func2 = {
      Iterator("void " + serializer + "::Write(const " + name + "& value, openpal::WriteBuffer& buff) const") ++ bracket {
        Iterator(fs.name+"::Write(Convert" + fs.name + "::Apply(value), buff);")
      }
    }

    singleton ++ space ++ func1 ++ space ++ func2
  }

}

object ConversionHeaders {

  val dataTypes = "<opendnp3/DataTypes.h>"
  val crob = "<opendnp3/ControlRelayOutputBlock.h>"
  val ao = "<opendnp3/AnalogOutput.h>"
  val factory = quoted("MeasurementFactory.h")
  val serializer = quoted("IDNP3Serializer.h")
  val conversions = quoted("../WriteConversions.h")

  val cppIncldues = List(factory, conversions)
}

import ConversionHeaders._

object BinaryConversion extends ArbitraryConversion("Binary", List(serializer, dataTypes), cppIncldues)
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

