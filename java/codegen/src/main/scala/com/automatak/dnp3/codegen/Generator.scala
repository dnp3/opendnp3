package com.automatak.dnp3.codegen

import java.nio.file.FileSystems

import com.automatak.dnp3._

object Generator {

  implicit val indent = CppIndentation

  val javaGenPath = FileSystems.getDefault.getPath("./cpp/adapters/JNIStrings.h");

  // all the classes to generate C++ info on
  def classes : List[ClassConfig] = List(
    ClassConfig(classOf[MasterStackConfig], true, false, false),
    ClassConfig(classOf[MasterConfig], true, false, false),
    ClassConfig(classOf[LinkLayerConfig], true, false, false),
    ClassConfig(classOf[LogEntry], false, false, true),
    ClassConfig(classOf[LogHandler], false, true, false)
  )

  def main(args: Array[String]): Unit = {

    writeTo(javaGenPath)(ClassInfoGenerator.lines(classes))
  }



}
