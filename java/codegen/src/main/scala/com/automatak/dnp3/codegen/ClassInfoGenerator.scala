package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Field, Method}

case class ClassConfig(clazz: Class[_], fields: Boolean, methods: Boolean, constructors: Boolean)

object ClassInfoGenerator {

  def lines(classes: List[ClassConfig])(implicit indent: Indentation) : Iterator[String] = {

    commented(LicenseHeader()) ++ space ++
    includeGuards("JNIStrings") {
      "struct Method { const char* name; const char* sig; };".iter ++
       space ++ classes.map(lines).flatten
    }

  }

  private def lines(cfg: ClassConfig)(implicit indent: Indentation) : Iterator[String] = {

    def fieldLine(f: Field) : String = "static const char* %s = \"%s\";".format(f.getName, f.getName)

    def methodLine(m: Method) : String = "static const Method %s = { \"%s\", \"%s\" };".format(m.getName, m.getName, m.jniSignature)

    def constructorLine(c: Constructor[_]) : String = "static const char* %s = \"%s\";".format("sig%s".format(0), c.jniSignature)

    def fqcn : String = "static const char* fqcn = \"%s\";".format(cfg.clazz.fqcn)

    def fields : Iterator[String] = {
      if(cfg.fields) {
        space ++ namespace("fields") {
          cfg.clazz.getFields.map(fieldLine).toIterator
        }
      }
      else Iterator.empty
    }

    def methods : Iterator[String] = {
      if(cfg.methods) {
        space ++ namespace("methods") {
          cfg.clazz.getDeclaredMethods.map(methodLine).toIterator
        }
      }
      else Iterator.empty
    }

    def constructors : Iterator[String] = {
      if(cfg.constructors) {
        space ++ namespace("constructors") {
          cfg.clazz.getConstructors.map(constructorLine).toIterator
        }
      }
      else Iterator.empty
    }

    namespace("classes") {
      namespace(cfg.clazz.getSimpleName) {
        fqcn.iter ++
        fields ++
        methods ++
        constructors
      }
    }

  }



}
