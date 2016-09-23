package com.automatak.dnp3.codegen

import java.nio.file.FileSystems

import com.automatak.dnp3._
import com.automatak.dnp3.enums._

object Generator {

  implicit val indent = CppIndentation

  val javaGenPath = FileSystems.getDefault.getPath("./cpp/adapters/JNIStrings.h");

  // all the classes to generate C++ info on
  def classes : List[ClassConfig] = List(

    ClassConfig(classOf[MasterStackConfig], Features.Fields),
    ClassConfig(classOf[MasterConfig], Features.FQCN, Features.Fields),
    ClassConfig(classOf[LinkLayerConfig], Features.FQCN, Features.Fields),
    ClassConfig(classOf[LogEntry], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[LogHandler], Features.Methods),
    ClassConfig(classOf[TimeSyncMode], Features.FQCN),
    ClassConfig(classOf[java.time.Duration], Features.FQCN),
    ClassConfig(classOf[ClassField], Features.FQCN, Features.Fields),
    ClassConfig(classOf[SOEHandler], Features.Methods),
    ClassConfig(classOf[HeaderInfo], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[GroupVariation], Features.FQCN, Features.Methods),
    ClassConfig(classOf[QualifierCode], Features.FQCN, Features.Methods),
    ClassConfig(classOf[TimestampMode], Features.FQCN, Features.Methods),
    ClassConfig(classOf[IndexedValue[_]], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[BinaryInput], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[DoubleBitBinaryInput], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[AnalogInput], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[Counter], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[FrozenCounter], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[BinaryOutputStatus], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[AnalogOutputStatus], Features.FQCN, Features.Constructors),
    ClassConfig(classOf[DoubleBit], Features.FQCN)

  )

  def main(args: Array[String]): Unit = {

    writeTo(javaGenPath)(ClassInfoGenerator.lines(classes))
  }



}
