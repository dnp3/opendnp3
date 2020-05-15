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

case class JNIWrapperGenerator(classes: List[Class[_]]) {

  def headerFileName : String = "JNIWrappers.h"

  def header(implicit indent: Indentation): Iterator[String] = {

    commented(LicenseHeader()) ++ space ++
      includeGuards("OPENDNP3_JNIWRAPPERS_H") {
          Iterator("#include <jni.h>") ++ space ++
          namespace("jni") {
            classes.flatMap(c => getWrapper(c)).toIterator
          }
      }
  }

  private def getWrapper(clazz: Class[_])(implicit indent: Indentation): Iterator[String] = {
      structDef(clazz.wrapperName) {
        Iterator(s"${clazz.wrapperName}(jobject value) : value(value) {}") ++ space ++
        Iterator("operator jobject() const { return value; }") ++ space ++
        Iterator("jobject value;")
      } ++ space
  }

}
