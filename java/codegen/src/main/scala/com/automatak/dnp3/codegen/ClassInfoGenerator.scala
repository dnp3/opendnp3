package com.automatak.dnp3.codegen

import java.lang.reflect.Field


object ClassInfoGenerator {

  def lines(classes: List[Class[_]])(implicit indent: Indentation) : Iterator[String] = {

    commented(LicenseHeader()) ++ space ++
    includeGuards("JNIStrings") {
       space ++ classes.map(lines).flatten
    }

  }

  private def lines(clazz: Class[_])(implicit indent: Indentation) : Iterator[String] = {

    def fieldLine(f: Field) : String = "static const char* %s = \"%s\";".format(f.getName, f.getName)
    def fqcn : String = "static const char* fqcn = \"%s\";".format(clazz.getName)

    def fields = clazz.getFields.map(fieldLine).toIterator

    namespace("classes") {
      namespace(clazz.getSimpleName) {
        fqcn.iter ++ space ++
        namespace("fields") {
          fields
        }
      }
    }

  }



}
