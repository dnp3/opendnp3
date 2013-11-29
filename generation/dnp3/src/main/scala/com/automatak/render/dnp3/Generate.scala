package com.automatak.render.dnp3

import com.automatak.render.dnp3.enums.{QualifierCode, FunctionCode}

import com.automatak.render.cpp.{CppIndentation, EnumModelRenderer}

object Generate {

  def main(args: Array[String]): Unit = {

    // list of all enumerations that we want to generate
    val enums = List(
      FunctionCode(),
      QualifierCode()
    )

    enums.foreach { e =>

     EnumModelRenderer(CppIndentation()).render(e).foreach(println)

    }

  }

}
