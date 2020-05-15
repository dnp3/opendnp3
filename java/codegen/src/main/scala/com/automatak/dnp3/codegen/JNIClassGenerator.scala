/*
 * Copyright 2013-2020 Automatak, LLC
 *
 * Licensed to Green Energy Corp (www.greenenergycorp.com) and Automatak
 * LLC (www.automatak.com) under one or more contributor license agreements. 
 * See the NOTICE file distributed with this work for additional information
 * regarding copyright ownership. Green Energy Corp and Automatak LLC license
 * this file to you under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
      space ++ "// method ids".iter ++ cfg.methods.zipWithIndex.map { case(_, id) => s"jmethodID method$id = nullptr;" }.toIterator
    }

    def constructorSignatures: Iterator[String] = cfg.ifEnabled(Features.Constructors) {
      space ++ "// constructor methods".iter ++ cfg.constructors.map(c => JNIMethod.getConstructorSignature(c)+";").toIterator
    }

    def constructorMembers: Iterator[String] = cfg.ifEnabled(Features.Constructors) {
      space ++ "// constructor method ids".iter ++ cfg.constructors.zipWithIndex.map(c => "jmethodID constructor%d = nullptr;".format(c._2)).toIterator
    }

    def fieldMembers: Iterator[String] = cfg.ifEnabled(Features.Fields) {
      space ++ "// field ids".iter ++ cfg.clazz.getFields.map(f => "jfieldID %sField = nullptr;".format(f.getName)).toIterator
    }

    def fieldGetters: Iterator[String] = cfg.ifEnabled(Features.Fields) {

      def impl(field : Field) : Iterator[String] = {
        if(field.getType.isPrimitive) {
          s"${JNIMethod.getType(field.getType)} get${field.getName}(JNIEnv* env, ${field.getDeclaringClass.wrapperName} instance);".iter
        } else {
          s"LocalRef<${JNIMethod.getType(field.getType)}> get${field.getName}(JNIEnv* env, ${field.getDeclaringClass.wrapperName} instance);".iter
        }

      }

      space ++ "// field getter methods".iter ++ cfg.fields.flatMap(impl)
    }

    def initSignature: Iterator[String] = "bool init(JNIEnv* env);".iter

    def cleanupSignature: Iterator[String] = "void cleanup(JNIEnv* env);".iter

    commented(LicenseHeader()) ++ space ++
      includeGuards("JNI%s".format(cfg.clazz.getSimpleName)) {
        """#include "../adapters/LocalRef.h"""".iter ++ space ++
        """#include "JNIWrappers.h"""".iter ++ space ++
        namespace("jni") {
          "struct JCache;".iter ++ space ++
          namespace("cache") {
            classDef(cfg.clazz.getSimpleName) {
              "friend struct jni::JCache;".iter ++ space ++
                initSignature ++ cleanupSignature ++ space ++
                "public:".iter ++
                constructorSignatures ++ methodSignatures ++ fieldGetters ++
                space ++ "private:".iter ++ space ++
                classMember ++ constructorMembers ++ methodsMembers ++ fieldMembers
            }
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

        def lines(c : Constructor[_], id: Int) : Iterator[String] = {
          setAndCheckReturn(s"this->constructor$id") {
            "env->GetMethodID(this->clazz, \"<init>\", \"%s\")".format(c.jniSignature)
          }
        }

        cfg.constructors.toIterator.zipWithIndex.flatMap { case(c, id) => lines(c, id) }
      }

      def methodInit : Iterator[String] = cfg.ifEnabled(Features.Methods) {

        def lines(id: Int, m : Method) : Iterator[String] = {

          def typ = if(m.isStatic) "Static" else ""

          setAndCheckReturn(s"this->method$id") {
              "env->Get%sMethodID(this->clazz, \"%s\", \"%s\")".format(typ, m.getName, m.jniSignature)
          }
        }

        cfg.methods.toIterator.zipWithIndex.flatMap{ case(m, id) => lines(id, m) }
      }

      def fieldInit : Iterator[String] = cfg.ifEnabled(Features.Fields) {

        def lines(f : Field) : Iterator[String] = {
          setAndCheckReturn("this->%sField".format(f.getName)) {
            "env->GetFieldID(this->clazz, \"%s\", \"%s\")".format(f.getName, JNIMethod.getFieldType(f.getType))
          }
        }

        cfg.clazz.getFields.toIterator.flatMap(lines)
      }

      "bool %s::init(JNIEnv* env)".format(cfg.clazz.getSimpleName).iter ++ bracket {

        "auto clazzTemp = env->FindClass(\"%s\");".format(cfg.jniClassName).iter ++
        "if(!clazzTemp) return false;".iter ++
        "this->clazz = (jclass) env->NewGlobalRef(clazzTemp);".iter ++
        "env->DeleteLocalRef(clazzTemp);".iter ++
        constructorInit ++ methodInit ++ fieldInit ++
        space ++ "return true;".iter
      }
    }

    def cleanupImpl : Iterator[String] = {
      "void %s::cleanup(JNIEnv* env)".format(cfg.clazz.getSimpleName).iter ++ bracket {
        "env->DeleteGlobalRef(this->clazz);".iter
      }
    }

    def methodsImpls : Iterator[String] = cfg.ifEnabled(Features.Methods) {
      cfg.methods.toIterator.zipWithIndex.flatMap { case(m, id) =>
        space ++ JNIMethod.getImpl(id, m)
      }
    }

    def constructorImpls : Iterator[String] = cfg.ifEnabled(Features.Constructors) {
      cfg.constructors.zipWithIndex.toIterator.flatMap { case(c, id) =>
        space ++ JNIMethod.getConstructorImpl(id, c)
      }
    }

    def fieldGetterImpls : Iterator[String] = cfg.ifEnabled(Features.Fields) {
      cfg.fields.toIterator.flatMap { f =>
        space ++ JNIMethod.getFieldGetterImpl(f)
      }
    }

    commented(LicenseHeader()) ++ space ++
    "#include \"%s\"".format(headerFileName).iter ++ space ++
    namespace("jni") {
      namespace("cache") {
        initImpl ++ space ++ cleanupImpl ++ methodsImpls ++ constructorImpls ++ fieldGetterImpls
      }
    }
  }

}
