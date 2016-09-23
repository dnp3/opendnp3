package com.automatak.dnp3.codegen

/// a cache for class names
object Classes {

  case class Record(cname: String, fqcn: String)

  def add(clazz: Class[_]): Unit = {
    classes = classes + clazz
  }

  private var classes : Set[Class[_]] = Set.empty

}
