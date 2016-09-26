package com.automatak.dnp3.codegen

import java.nio.file.FileSystems

import com.automatak.dnp3._
import com.automatak.dnp3.enums._

object Generator {


  def enumeration(clazz: Class[_]): ClassConfig = {
    ClassConfig(
      clazz,
      Set(Features.Methods),
      MethodFilter.any(MethodFilter.nameEquals("fromType"), MethodFilter.nameEquals("toType"))
    )
  }

  def listMethods = MethodFilter.nameEquals("add", Some(1))
  def listConstructors = ConstructorFilter.withParamTypes(List("int"))

  implicit val indent = CppIndentation

 // val javaGenPath = FileSystems.getDefault.getPath("./cpp/adapters/JNIStrings.h");

  val javaJNIPath = FileSystems.getDefault.getPath("./cpp/jni/");

  // all the classes to generate C++ info on
  def classes : List[ClassConfig] = List(
    ClassConfig(classOf[MasterStackConfig], Set(Features.Fields)),
    ClassConfig(classOf[MasterConfig], Set(Features.Fields)),
    ClassConfig(classOf[LinkLayerConfig], Set(Features.Fields)),
    ClassConfig(classOf[LogEntry], Set(Features.Constructors)),
    ClassConfig(classOf[LogHandler], Set(Features.Methods)),
    ClassConfig(classOf[ClassField], Set(Features.Fields)),
    ClassConfig(classOf[SOEHandler], Set(Features.Methods)),
    ClassConfig(classOf[HeaderInfo], Set(Features.Constructors)),
    enumeration(classOf[GroupVariation]),
    enumeration(classOf[QualifierCode]),
    enumeration(classOf[TimestampMode]),
    ClassConfig(classOf[IndexedValue[_]], Set(Features.Constructors)),
    ClassConfig(classOf[BinaryInput], Set(Features.Constructors)),
    ClassConfig(classOf[DoubleBitBinaryInput], Set(Features.Constructors)),
    ClassConfig(classOf[AnalogInput], Set(Features.Constructors)),
    ClassConfig(classOf[Counter], Set(Features.Constructors)),
    ClassConfig(classOf[FrozenCounter], Set(Features.Constructors)),
    ClassConfig(classOf[BinaryOutputStatus], Set(Features.Constructors)),
    ClassConfig(classOf[AnalogOutputStatus], Set(Features.Constructors)),
    ClassConfig(classOf[java.time.Duration], Set(Features.Methods), MethodFilter.nameEquals("toMillis")),
    ClassConfig(classOf[java.util.ArrayList[_]], Set(Features.Constructors, Features.Methods), listMethods, listConstructors),
    enumeration(classOf[DoubleBit]),
    enumeration(classOf[TimeSyncMode]),
    ClassConfig(classOf[MasterApplication], Set(Features.Methods)),
    ClassConfig(classOf[IINField], Set(Features.Constructors)),
    ClassConfig(classOf[TaskConfig], Set(Features.Constructors)),
    enumeration(classOf[MasterTaskType]),
    ClassConfig(classOf[TaskId], Set(Features.Constructors)),
    ClassConfig(classOf[TaskInfo], Set(Features.Constructors)),
    enumeration(classOf[TaskCompletion]),
    ClassConfig(classOf[java.lang.Iterable[_]], Set(Features.Methods), MethodFilter.nameEquals("iterator")),
    ClassConfig(classOf[java.util.Iterator[_]], Set(Features.Methods), MethodFilter.equalsAny("hasNext", "next")),
    ClassConfig(classOf[ClassAssignment], Set(Features.Fields))
  )

  def main(args: Array[String]): Unit = {

   // JNIClassGenerator(ClassConfig(classOf[MasterConfig], Set(Features.Fields))).impl.foreach(println)

    classes.foreach { c =>

      val gen = JNIClassGenerator(c)

      writeTo(javaJNIPath.resolve(gen.headerFileName))(gen.header)
      writeTo(javaJNIPath.resolve(gen.implFileName))(gen.impl)
    }

    val staticClass = JCacheGenerator(classes)

    writeTo(javaJNIPath.resolve(staticClass.headerFileName))(staticClass.header)
    writeTo(javaJNIPath.resolve(staticClass.implFileName))(staticClass.impl)

  }



}
