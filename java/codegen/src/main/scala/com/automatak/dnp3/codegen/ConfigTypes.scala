package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Field, Method, Modifier}

import scala.language.existentials

object Features extends Enumeration {
  val Fields, Methods, Constructors = Value
}

trait MethodFilter {
  def matches(method: Method) : Boolean
}

object MethodFilter {

  def acceptsAny : MethodFilter = _ => true

  def nameEquals(string: String, args: Option[Int] = None) : MethodFilter =  {
    method: Method => method.getName == string && args.forall(_ == method.getParameterCount)
  }

  def any(filters: MethodFilter*) : MethodFilter =  {
    method: Method => filters.exists(_.matches(method))
  }

  def equalsAny(names: String*) : MethodFilter = {
    method: Method => names.contains(method.getName)
  }
}

trait ConstructorFilter {
  def matches(c: Constructor[_]) : Boolean
}

object ConstructorFilter {

  def acceptsAny : ConstructorFilter = (c: Constructor[_]) => true

  def withParamTypes(params : List[String]) : ConstructorFilter = {
    c: Constructor[_] => c.getParameters.map(p => p.getType.getName).toList == params
  }

}

case class ClassConfig(clazz: Class[_], features : Set[Features.Value], mfilter: MethodFilter = MethodFilter.acceptsAny, cfilter: ConstructorFilter = ConstructorFilter.acceptsAny) {

  def isEnabled(f : Features.Value) : Boolean  = features.contains(f)

  def ifEnabled(f : Features.Value)(inner: => Iterator[String]) : Iterator[String] = {
    if(!isEnabled(f)) Iterator.empty else {
      inner
    }
  }

  def methods : Array[Method] = clazz.getDeclaredMethods.filter(mfilter.matches).sortWith(_.getName < _.getName)

  def constructors : Array[Constructor[_]] = clazz.getConstructors.filter(cfilter.matches).sortWith(_.getName < _.getName)

  def fields : Array[Field] = clazz.getDeclaredFields.filter(f => !Modifier.isStatic(f.getModifiers)).sortWith(_.getName < _.getName)
}

