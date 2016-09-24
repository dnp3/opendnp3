package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Field, Method}

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

object ClassInfoGenerator {

  def lines(classes: List[ClassConfig])(implicit indent: Indentation) : Iterator[String] = {

    commented(LicenseHeader()) ++ space ++
    includeGuards("JNIStrings") {
      List("#include \"JNITypes.h\"", "#include \"AdditionalJNIStrings.h\"").toIterator ++
       space ++ classes.map(lines).flatten
    }

  }

  /*
  namespace constructors {
    namespace init3 {
        static const MethodInfo sig = { "<init>", "(DBJ)V" };
        jmethodID getId(JNIEnv* env);
        jobject call(JNIEnv* env, jmethodID id, jdouble value, jbyte quality, jlong timestamp);
    }
  }
  */

  private def lines(cfg: ClassConfig)(implicit indent: Indentation) : Iterator[String] = {

    def fieldLine(f: Field) : String = "static const FieldId %s = { \"%s\" };".format(f.getName, f.getName)

    def methodLine(m: Method) : String = "static const MethodInfo %s = { \"%s\", \"%s\" };".format(m.getName, m.getName, m.jniSignature)

    def constructorLine(c: Constructor[_]) : String = {
      "static const MethodInfo %s = { \"%s\", \"%s\" };".format("init%s".format(c.getParameterCount), "<init>", c.jniSignature)
    }

    def fqcn : Iterator[String] = {
        "static const FQCN fqcn = { \"%s\" };".format(cfg.clazz.fqcn).iter
    }

    def fields : Iterator[String] = {
      if(cfg.isEnabled(Features.Fields)) {
        space ++ namespace("fields") {
          cfg.clazz.getFields.map(fieldLine).toIterator
        }
      }
      else Iterator.empty
    }

    def methods : Iterator[String] = {
      if(cfg.isEnabled(Features.Methods)) {
        space ++ namespace("methods") {
          cfg.clazz.getDeclaredMethods.map(methodLine).toIterator
        }
      }
      else Iterator.empty
    }

    def constructors : Iterator[String] = {
      if(cfg.isEnabled(Features.Constructors)) {
        space ++ namespace("constructors") {
          cfg.clazz.getConstructors.map(constructorLine).toIterator
        }
      }
      else Iterator.empty
    }

    namespace("classes") {
      namespace(cfg.clazz.getSimpleName) {
        fqcn ++
        fields ++
        methods ++
        constructors
      }
    }

  }



}
