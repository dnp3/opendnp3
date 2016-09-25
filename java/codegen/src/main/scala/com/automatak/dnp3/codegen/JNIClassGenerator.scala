package com.automatak.dnp3.codegen

import java.lang.reflect.{Constructor, Field, Method}


case class JNIClassGenerator(cfg: ClassConfig) {

  def headerFileName : String = "JNI%s.h".format(cfg.clazz.getSimpleName)

  def implFileName : String = "JNI%s.cpp".format(cfg.clazz.getSimpleName)

  def header(implicit indent: Indentation): Iterator[String] = {

    def classMember: Iterator[String] = "jclass clazz = nullptr;".iter

    def methodSignatures: Iterator[String] = cfg.ifEnabled(Features.Methods) {
      space ++ "// methods".iter ++ cfg.methods.map(m => "%s;".format(JNIMethod.getSignature(m))).toIterator
    }

    def methodsMembers: Iterator[String] = cfg.ifEnabled(Features.Methods) {
      space ++ "// method ids".iter ++ cfg.methods.map(m => "jmethodID %sMethod = nullptr;".format(m.getName)).toIterator
    }

    def constructorSignatures: Iterator[String] = cfg.ifEnabled(Features.Constructors) {
      space ++ "// constructor methods".iter ++ cfg.clazz.getConstructors.map(c => JNIMethod.getConstructorSignature(c)+";").toIterator
    }

    def constructorMembers: Iterator[String] = cfg.ifEnabled(Features.Constructors) {
      space ++ "// constructor method ids".iter ++ cfg.clazz.getConstructors.map(c => "jmethodID init%dConstructor = nullptr;".format(c.getParameterCount)).toIterator
    }

    def fieldMembers: Iterator[String] = cfg.ifEnabled(Features.Fields) {
      space ++ "// field ids".iter ++ cfg.clazz.getDeclaredFields.map(f => "jfieldID %sField = nullptr;".format(f.getName)).toIterator
    }

    def fieldGetters: Iterator[String] = cfg.ifEnabled(Features.Fields) {
      space ++ "// field getter methods".iter ++ cfg.clazz.getDeclaredFields.map(f => "%s get%s(JNIEnv* env);".format(JNIMethod.getType(f.getType), f.getName)).toIterator
    }

    def initSignature: Iterator[String] = "bool init(JNIEnv* env);".iter

    commented(LicenseHeader()) ++ space ++
      includeGuards("JNI%s".format(cfg.clazz.getSimpleName)) {
        "#include <jni.h>".iter ++ space ++
        namespace("jni") {
          classDef(cfg.clazz.getSimpleName) {
              "public:".iter ++ space ++
              initSignature ++
              constructorSignatures ++ methodSignatures ++ fieldGetters ++
              space ++ "private:".iter ++ space ++
              classMember ++ constructorMembers ++ methodsMembers ++ fieldMembers
          }
        }
      }
  }

  def impl(implicit indent: Indentation): Iterator[String] = {

    def initImpl: Iterator[String] = {

      def setAndCheckReturn(name: String)(value: String) : Iterator[String] = {
        space ++
        "%s = %s;".format(name, value).iter ++
        "if(!%s) return false;".format(name).iter
      }

      def constructorInit : Iterator[String] = cfg.ifEnabled(Features.Constructors) {

        def lines(c : Constructor[_]) : Iterator[String] = {
          setAndCheckReturn("this->init%dConstructor".format(c.getParameterCount)) {
            "env->GetMethodID(this->clazz, \"<init>\", \"%s\")".format(c.jniSignature)
          }
        }

        cfg.clazz.getConstructors.toIterator.map(lines).flatten
      }

      def methodInit : Iterator[String] = cfg.ifEnabled(Features.Methods) {

        def lines(m : Method) : Iterator[String] = {
          setAndCheckReturn("this->%sMethod".format(m.getName)) {
            "env->GetMethodID(this->clazz, \"%s\", \"%s\")".format(m.getName, m.jniSignature)
          }
        }

        cfg.methods.toIterator.map(lines).flatten
      }

      def fieldInit : Iterator[String] = cfg.ifEnabled(Features.Fields) {

        def lines(f : Field) : Iterator[String] = {
          setAndCheckReturn("this->%sField".format(f.getName)) {
            "env->GetFieldID(this->clazz, \"%s\", \"%s\")".format(f.getName, JNIMethod.getFieldType(f.getType))
          }
        }

        cfg.clazz.getFields.toIterator.map(lines).flatten
      }

      "bool %s::init(JNIEnv* env)".format(cfg.clazz.getSimpleName).iter ++ bracket {

        setAndCheckReturn("this->clazz") {
          "env->FindClass(\"%s\")".format(cfg.clazz.fqcn)
        } ++
        constructorInit ++ methodInit ++ fieldInit ++
        space ++ "return true;".iter
      }
    }

    def methodsImpls : Iterator[String] = cfg.ifEnabled(Features.Methods) {
      cfg.methods.toIterator.flatMap { m =>
        space ++ JNIMethod.getImpl(m)
      }
    }

    commented(LicenseHeader()) ++ space ++
    "#include \"%s\"".format(headerFileName).iter ++ space ++
    "#include \"JNI.h\"".iter ++ "#include <assert.h>".iter ++ space ++
    namespace("jni") {
      initImpl ++ methodsImpls
    } ++ space
  }

}
