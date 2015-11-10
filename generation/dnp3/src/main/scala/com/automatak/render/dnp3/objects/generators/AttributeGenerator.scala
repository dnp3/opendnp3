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

    case class AttrConfig(signature: String, attr: FieldAttribute.Value)

    val hasAbsTime = AttrConfig("bool HasAbsoluteTime(GroupVariation gv)", FieldAttribute.IsTimeUTC)
    val hasRelTime = AttrConfig("bool HasRelativeTime(GroupVariation gv)", FieldAttribute.IsTimeRel)
    val hasFlags = AttrConfig("bool HasFlags(GroupVariation gv)", FieldAttribute.IsFlags)

    val attributes : List[AttrConfig] = List(hasAbsTime, hasRelTime, hasFlags)

    def license = commented(LicenseHeader())

    def writeHeader() {

      def includes : Iterator[String] = Iterator(include(""""opendnp3/gen/GroupVariation.h""""))

      def signatures : Iterator[String] = attributes.map(a => a.signature + ";").iterator

      def lines = license ++ space ++ includeGuards(name)(includes ++ space ++ namespace(cppNamespace)(signatures))

      writeTo(headerPath)(lines)
    }

    def writeImpl() {
      def license = commented(LicenseHeader())

      def cases(attr: FieldAttribute.Value) : Iterator[String] = ObjectGroup.all.map(og => getCases(og, attr)).flatten.iterator

      def getCases(og: ObjectGroup, attr: FieldAttribute.Value) : List[String] = og.objects.flatMap(gv => getCase(gv, attr)).flatten

      def getCase(gv: GroupVariation, attr: FieldAttribute.Value) : Option[Iterator[String]] = {
        if(gv.attributes.contains(attr))
        {
            Some(Iterator("case(GroupVariation::%s):".format(gv.name)) ++ indent {
              Iterator("return true;")
            })
        }
        else None
      }

      def measImpl(attr: AttrConfig) : Iterator[String] = Iterator(attr.signature) ++ bracket {
        Iterator(String.format("switch(gv)")) ++ bracket {
          cases(attr.attr) ++
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
