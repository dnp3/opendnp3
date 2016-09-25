package com.automatak.dnp3.codegen

case class JCacheGenerator(classes: List[ClassConfig]) {

  def headerFileName : String = "JCache.h"

  def implFileName : String = "JCache.cpp"

  def header(implicit indent: Indentation): Iterator[String] = {

    def classIncludes : Iterator[String] = classes.map(c => "#include \"JNI%s.h\"".format(c.clazz.getSimpleName)).toIterator

    def instances : Iterator[String] = classes.map(c => "static jni::%s %s;".format(c.clazz.getSimpleName, c.clazz.getSimpleName.decapitalize)).toIterator

    commented(LicenseHeader()) ++ space ++
      includeGuards("OPENDNP3_JNITYPES") {
        "#include \"openpal/util/Uncopyable.h\"".iter ++ space ++
        classIncludes ++ space ++
          namespace("jni") {
            structDef("JCache: private openpal::StaticOnly") {
              "static bool init(JNIEnv* env);".iter ++ space ++
                instances
            }
          }
      }
  }

  def impl(implicit indent: Indentation): Iterator[String] = {

    def instances : Iterator[String] = {
      classes.iterator.flatMap {
        c => {
          "success = %s.init(env);".format(c.clazz.getSimpleName.decapitalize).iter ++
          "if(!success) return false;".iter
        }
      }
    }

    def jcacheInit = {
      "bool JCache::init(JNIEnv* env)".iter ++ bracket {
        "auto success = true;".iter ++ space ++
          instances ++
          "return true;".iter
      }
    }

    def staticInitializers : Iterator[String] = {
      classes.iterator.flatMap { c =>
        "jni::%s JCache::%s;".format(c.clazz.getSimpleName, c.clazz.getSimpleName.decapitalize).iter
      }
    }

    commented(LicenseHeader()) ++ space ++
      "#include \"JCache.h\"".iter ++ space ++
      namespace("jni") {
        staticInitializers ++ space ++ jcacheInit
      }
  }

}
