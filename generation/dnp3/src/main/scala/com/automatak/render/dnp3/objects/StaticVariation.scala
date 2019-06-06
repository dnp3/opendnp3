package com.automatak.render.dnp3.objects

import com.automatak.render.Indentation

trait StaticVariation extends GroupVariation {
  def staticEnumName : String
  override def headerLines(implicit i : Indentation) : Iterator[String] = {
    super.headerLines ++ Iterator("static const %s svariation = %s::Group%dVar%d;".format(staticEnumName, staticEnumName, group, variation))
  }
}

object StaticVariation {

  trait Binary extends StaticVariation {
    override def staticEnumName : String = "StaticBinaryVariation"
  }

  trait DoubleBinary extends StaticVariation {
    override def staticEnumName : String = "StaticDoubleBinaryVariation"
  }

  trait Analog extends StaticVariation {
    override def staticEnumName : String = "StaticAnalogVariation"
  }

  trait Counter extends StaticVariation {
    override def staticEnumName : String = "StaticCounterVariation"
  }

  trait FrozenCounter extends StaticVariation {
    override def staticEnumName : String = "StaticFrozenCounterVariation"
  }

  trait BinaryOutputStatus extends StaticVariation {
    override def staticEnumName : String = "StaticBinaryOutputStatusVariation"
  }

  trait AnalogOutputStatus extends StaticVariation {
    override def staticEnumName : String = "StaticAnalogOutputStatusVariation"
  }

  trait TimeAndInterval extends StaticVariation {
    override def staticEnumName : String = "StaticTimeAndIntervalVariation"
  }
}

