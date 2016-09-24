package com.automatak.dnp3.codegen

import java.lang.reflect.Method

object Features extends Enumeration {
  val Fields, Methods, Constructors = Value
}

trait MethodFilter {
  def matches(method: Method) : Boolean
}

object AcceptsAnyFilter extends MethodFilter {
  override def matches(method: Method) : Boolean = true
}

case class MethodEquals(string: String) extends MethodFilter {
  override def matches(method: Method) : Boolean = {
    method.getName == string
  }
}

case class ClassConfig(clazz: Class[_], features : Set[Features.Value], mfilter: MethodFilter = AcceptsAnyFilter) {

  def isEnabled(f : Features.Value) : Boolean  = features.contains(f)

  def ifEnabled(f : Features.Value)(inner: => Iterator[String]) : Iterator[String] = {
    if(!isEnabled(f)) Iterator.empty else {
      inner
    }
  }

  def methods : Array[Method] = clazz.getDeclaredMethods.filter(mfilter.matches)
}

