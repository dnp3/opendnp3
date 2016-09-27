package com.automatak.dnp3.codegen

import java.nio.file.FileSystems
import java.util.concurrent.CompletableFuture

import com.automatak.dnp3._
import com.automatak.dnp3.enums._
import com.automatak.dnp3.impl.CommandBuilderImpl

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
    ClassConfig(classOf[IndexedValue[_]], Set(Features.Constructors, Features.Fields)),
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
    enumeration(classOf[MasterTaskType]),
    ClassConfig(classOf[TaskId], Set(Features.Constructors)),
    ClassConfig(classOf[TaskInfo], Set(Features.Constructors)),
    enumeration(classOf[TaskCompletion]),
    ClassConfig(classOf[java.lang.Iterable[_]], Set(Features.Methods), MethodFilter.nameEquals("iterator")),
    ClassConfig(classOf[java.util.Iterator[_]], Set(Features.Methods), MethodFilter.equalsAny("hasNext", "next")),
    ClassConfig(classOf[ClassAssignment], Set(Features.Fields)),
    ClassConfig(classOf[Range], Set(Features.Fields, Features.Methods), MethodFilter.nameEquals("isDefined")),
    ClassConfig(classOf[CommandProcessor], Set(Features.Methods)),
    ClassConfig(classOf[Stack], Set(Features.Methods)),
    ClassConfig(classOf[CommandBuilderImpl], Set(Features.Fields, Features.Constructors)),
    ClassConfig(classOf[CommandHeaders], Set(Features.Methods)),
    ClassConfig(classOf[CommandTaskResult], Set(Features.Constructors)),
    ClassConfig(classOf[CommandPointResult], Set(Features.Constructors)),
    enumeration(classOf[CommandPointState]),
    enumeration(classOf[CommandStatus]),
    ClassConfig(classOf[CompletableFuture[_]], Set(Features.Methods), MethodFilter.nameEquals("complete")),
    ClassConfig(classOf[ControlRelayOutputBlock], Set(Features.Fields)),
    ClassConfig(classOf[AnalogOutputInt16], Set(Features.Fields)),
    ClassConfig(classOf[AnalogOutputInt32], Set(Features.Fields)),
    ClassConfig(classOf[AnalogOutputFloat32], Set(Features.Fields)),
    ClassConfig(classOf[AnalogOutputDouble64], Set(Features.Fields)),
    enumeration(classOf[ControlCode]),
    enumeration(classOf[ChannelState]),
    ClassConfig(classOf[ChannelListener], Set(Features.Methods))
  ).sortBy(_.clazz.getSimpleName)

  def main(args: Array[String]): Unit = {

    implicit val indent = CppIndentation

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
