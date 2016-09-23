package com.automatak.dnp3.codegen

import java.nio.file.FileSystems

import com.automatak.dnp3._

object Main {

  implicit val indent = CppIndentation

  val javaGenPath = FileSystems.getDefault.getPath("./cpp/adapters/JNIStrings.h");

  // all the classes to generate C++ info on
  def classes : List[Class[_]] = List(
    classOf[MasterConfig]
  )

  def main(args: Array[String]): Unit = {

    writeTo(javaGenPath)(ClassInfoGenerator.lines(classes))

  }



}
