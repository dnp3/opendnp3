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

case class JCacheGenerator(classes: List[ClassConfig]) {

  def headerFileName : String = "JCache.h"

  def implFileName : String = "JCache.cpp"

  def header(implicit indent: Indentation): Iterator[String] = {

    def classIncludes : Iterator[String] = classes.map(c => "#include \"JNI%s.h\"".format(c.clazz.getSimpleName)).toIterator

    def instances : Iterator[String] = classes.map(c => "static cache::%s %s;".format(c.clazz.getSimpleName, c.clazz.getSimpleName)).toIterator

    commented(LicenseHeader()) ++ space ++
      includeGuards("OPENDNP3_JNITYPES") {
        "#include \"opendnp3/util/StaticOnly.h\"".iter ++ space ++
        classIncludes ++ space ++
          namespace("jni") {
            structDef("JCache: private opendnp3::StaticOnly") {
              "static bool init(JNIEnv* env);".iter ++
                "static void cleanup(JNIEnv* env);".iter ++
                space ++
                instances
            }
          }
      }
  }

  def impl(implicit indent: Indentation): Iterator[String] = {

    def initAll : Iterator[String] = {
      val entries = classes.map {
        c => "%s.init(env)".format(c.clazz.getSimpleName)
      }
      
      ("return " + entries.head).iter ++ entries.tail.map(s => "&& " + s).iterator ++ ";".iter
    }

    def jcacheInit = {
      "bool JCache::init(JNIEnv* env)".iter ++ bracket {
        initAll
      }
    }

    def jcacheCleanup = {
      "void JCache::cleanup(JNIEnv* env)".iter ++ bracket {
        classes.iterator.flatMap { c =>
          "%s.cleanup(env);".format(c.clazz.getSimpleName).iter
        }
      }
    }

    def staticInitializers : Iterator[String] = {
      classes.iterator.flatMap { c =>
        "cache::%s JCache::%s;".format(c.clazz.getSimpleName, c.clazz.getSimpleName).iter
      }
    }

    commented(LicenseHeader()) ++ space ++
      "#include \"JCache.h\"".iter ++ space ++
      namespace("jni") {
        staticInitializers ++ space ++ jcacheInit ++ space ++ jcacheCleanup
      }
  }

}
