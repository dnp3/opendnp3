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

    val measSignature = "MeasAttr GetMeasAttr(GroupVariation gv)"

    def license = commented(LicenseHeader())

    def writeHeader() {

      def includes : Iterator[String] = Iterator(include(""""opendnp3/app/MeasAttr.h""""), include(""""opendnp3/gen/GroupVariation.h""""))

      def signatures : Iterator[String] = Iterator(measSignature+";")

      def lines = license ++ space ++ includeGuards(name)(includes ++ space ++ cstdint ++ space ++ namespace(cppNamespace)(signatures))

      writeTo(headerPath)(lines)
    }

    def writeImpl() {
      def license = commented(LicenseHeader())

      def cases : Iterator[String] = ObjectGroup.all.map(og => getCases(og)).flatten.iterator

      def getCases(og: ObjectGroup) : List[String] = og.objects.flatMap(gv => getCase(gv)).flatten

      def getCase(gv: GroupVariation) : Option[Iterator[String]] = {
        val attr : Set[FieldAttribute.Value] = gv.attributes
        val isTime = attr.contains(FieldAttribute.IsTimeUTC)
        val isFlag = attr.contains(FieldAttribute.IsFlags)

        if(isTime || isFlag)
        {
            Some(Iterator("case(GroupVariation::%s):".format(gv.name)) ++ indent {
              Iterator("return MeasAttr(%b, %b);".format(isTime, isFlag))
            })
        }
        else None
      }

      def measImpl : Iterator[String] = Iterator(measSignature) ++ bracket {
        Iterator(String.format("switch(gv)")) ++ bracket {
          cases ++
          Iterator("default:") ++ indent {
            Iterator("return MeasAttr(false, false);")
          }
        }
      }

      def lines = license ++ space ++ Iterator(include(quoted("opendnp3/gen/Attributes.h"))) ++ space ++ namespace(cppNamespace) {
        measImpl
      }

      writeTo(implPath)(lines)
      println("Wrote: " + implPath)
    }

    writeHeader()
    writeImpl()
  }


}
