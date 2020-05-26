/**
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
package com.automatak.render.cpp

import com.automatak.render._

object EnumSerialization extends ModelRenderer[EnumModel] {

  private def ser4cppNamespace = "namespace ser4cpp"
  private def customSerializersNamespace = "namespace serializers"
  private def namespaced(inner: Iterator[String])(implicit indent: Indentation) : Iterator[String] = {
    Iterator(ser4cppNamespace) ++ bracket {
      Iterator(customSerializersNamespace) ++ bracket {
        inner
      }
    }
  }

  private def template = "template<>"
  private def writeSignature(enum: EnumModel) = Iterator("template<>", "inline bool write_one(wseq_t& dest, const opendnp3::%s& value)".format(enum.name))
  private def readSignature(enum: EnumModel)  = Iterator("template<>", "inline bool read_one(rseq_t& input, opendnp3::%s& out)".format(enum.name))

  def render(em: EnumModel)(implicit i: Indentation) : Iterator[String] = {

    def writeOne = writeSignature(em) ++ bracket {
      Iterator(f"return ${em.enumType}::write_to(dest, opendnp3::${em.name}Spec::to_type(value));")
    }
    def readOne = readSignature(em) ++ bracket {
      val tempValueName = f"temp${em.name}"
      Iterator(
        f"${getEnumType(em.enumType)} $tempValueName;",
        f"bool result = ${em.enumType}::read_from(input, $tempValueName);",
        f"out = opendnp3::${em.name}Spec::from_type($tempValueName);",
        "return result;"
      )
    }

    namespaced(writeOne ++ space ++ readOne)
  }
}
