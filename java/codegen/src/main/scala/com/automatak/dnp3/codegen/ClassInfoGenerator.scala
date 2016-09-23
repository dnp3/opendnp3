package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Field, Method}

object Features extends Enumeration {
  val FQCN, Fields, Methods, Constructors = Value
}

object ClassConfig {
  def apply(clazz: Class[_], features: Features.Value*) : ClassConfig = ClassConfig(clazz, features.toSet)
}

case class ClassConfig(clazz: Class[_], features : Set[Features.Value]) {
  def isEnabled(f : Features.Value) : Boolean  = features.contains(f)
}

object ClassInfoGenerator {

  def lines(classes: List[ClassConfig])(implicit indent: Indentation) : Iterator[String] = {

    commented(LicenseHeader()) ++ space ++
    includeGuards("JNIStrings") {
      List("#include \"JNITypes.h\"", "#include \"AdditionalJNIStrings.h\"").toIterator ++
       space ++ classes.map(lines).flatten
    }

  }

  private def lines(cfg: ClassConfig)(implicit indent: Indentation) : Iterator[String] = {

    def fieldLine(f: Field) : String = "static const FieldId %s = { \"%s\" };".format(f.getName, f.getName)

    def methodLine(m: Method) : String = "static const MethodInfo %s = { \"%s\", \"%s\" };".format(m.getName, m.getName, m.jniSignature)

    def constructorLine(c: Constructor[_]) : String = {
      "static const MethodInfo %s = { \"%s\", \"%s\" };".format("init%s".format(c.getParameterCount), "<init>", c.jniSignature)
    }

    def fqcn : Iterator[String] = {
      if(cfg.isEnabled(Features.FQCN)) {
        "static const FQCN fqcn = { \"%s\" };".format(cfg.clazz.fqcn).iter
      }
      else Iterator.empty
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
