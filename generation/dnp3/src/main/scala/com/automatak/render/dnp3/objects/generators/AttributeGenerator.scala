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
package com.automatak.render.dnp3.objects.generators

import java.nio.file.Path

import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.LicenseHeader
import com.automatak.render.cpp.CppIndentation
import com.automatak.render.dnp3.objects.{FieldAttribute, ObjectGroup, GroupVariation}


object AttributeGenerator {
  implicit val indent = CppIndentation()


  def writeAttributes(cppNamespace: String, inc: Path, impl: Path): Unit = {

    val name = "Attributes"
    val headerPath = inc.resolve(String.format("%s.h", name))
    val implPath = impl.resolve(String.format("%s.cpp", name))

    trait HasAttribute {
      def signature : String
      def matches(gv: GroupVariation) : Boolean
    }

    case class HasFieldAttrib(signature: String, attr: FieldAttribute.Value) extends HasAttribute {
      def matches(gv: GroupVariation) : Boolean = gv.attributes.contains(attr)
    }

    object EventAttribute extends HasAttribute {
      def signature = "bool IsEvent(GroupVariation gv)";
      def matches(gv: GroupVariation) : Boolean = {
         gv.parent.isEventGroup && gv.variation != 0
      }
    }

    val attributes : List[HasAttribute] = List(
      HasFieldAttrib("bool HasAbsoluteTime(GroupVariation gv)", FieldAttribute.IsTimeUTC),
      HasFieldAttrib("bool HasRelativeTime(GroupVariation gv)", FieldAttribute.IsTimeRel),
      HasFieldAttrib("bool HasFlags(GroupVariation gv)", FieldAttribute.IsFlags),
      EventAttribute
    )

    def license = commented(LicenseHeader())

    def writeHeader() : Unit = {

      def includes : Iterator[String] = Iterator(include(""""opendnp3/gen/GroupVariation.h""""))

      def signatures : Iterator[String] = attributes.map(a => a.signature + ";").iterator

      def lines = license ++ space ++ includeGuards(name)(includes ++ space ++ namespace(cppNamespace)(signatures))

      writeTo(headerPath)(lines)
    }

    def writeImpl(): Unit = {
      def license = commented(LicenseHeader())

      def cases(attr: HasAttribute) : Iterator[String] = ObjectGroup.all.flatMap(og => getCases(og, attr)).iterator

      def getCases(og: ObjectGroup, attr: HasAttribute) : List[String] = og.objects.flatMap(gv => getCase(gv, attr)).flatten

      def getCase(gv: GroupVariation, attr: HasAttribute) : Option[Iterator[String]] = {
        if(attr.matches(gv))
        {
            Some(Iterator("case(GroupVariation::%s):".format(gv.name)) ++ indent {
              Iterator("return true;")
            })
        }
        else None
      }

      def measImpl(attr: HasAttribute) : Iterator[String] = Iterator(attr.signature) ++ bracket {
        Iterator(String.format("switch(gv)")) ++ bracket {
          cases(attr) ++
          Iterator("default:") ++ indent {
            Iterator("return false;")
          }
        }
      }

      def impls : Iterator[String] = attributes.flatMap(x => measImpl(x)).iterator

      def lines = license ++ space ++ Iterator(include(quoted("opendnp3/gen/Attributes.h"))) ++ space ++ namespace(cppNamespace) {
        impls
      }

      writeTo(implPath)(lines)
      println("Wrote: " + implPath)
    }

    writeHeader()
    writeImpl()
  }


}
