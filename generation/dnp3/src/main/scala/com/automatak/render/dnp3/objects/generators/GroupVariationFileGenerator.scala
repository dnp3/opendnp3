package com.automatak.render.dnp3.objects.generators

import java.nio.file.Path
import com.automatak.render.dnp3.objects._
import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.LicenseHeader

object GroupVariationFileGenerator {

  def apply(path: Path) = {

    implicit val indent = CppIndentation()

    def headerPath(group: ObjectGroup): Path = path.resolve(group.name+".h")
    def implPath(group: ObjectGroup): Path = path.resolve(group.name+".cpp")

    def definitions(r: ModelRenderer[GroupVariation])(group: ObjectGroup): Iterator[String] = spaced(group.objects.iterator.map(o => r.render(o)))

    def enumIncludes(group: ObjectGroup) : List[String] = {

      def getEnums(gv: GroupVariation):  Set[String] = {

        def extract(typ: FixedSizeFieldType): Option[String] = typ match {
          case EnumField(model) => Some("<opendnp3/gen/"+model.name+".h>")
          case _ => None
        }

        gv match {
          case fs : FixedSize => fs.fields.flatMap(f => extract(f.typ)).toSet
          case _ => Set.empty
        }
      }

      group.objects.flatMap(o => getEnums(o)).map(inc => "#include " + inc)
    }

    def includeHeader(group: ObjectGroup): Iterator[String] = Iterator("#include " + quoted(group.name+".h"))

    def headerFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
      Iterator("#include <openpal/BufferWrapper.h>") ++ enumIncludes(group) ++ space ++
      namespace("opendnp3") {
        definitions(GroupVariationHeaderRenderer)(group)
      }
    }

    def implFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
        includeHeader(group) ++ space ++
        Iterator("#include <openpal/Serialization.h>") ++ space ++
        Iterator("using namespace openpal;") ++ space ++
        namespace("opendnp3") {
          definitions(GroupVariationImplRenderer)(group)
        }
    }

    ObjectGroup.all.foreach(g => writeTo(headerPath(g))(headerFile(g)))
    ObjectGroup.all.foreach(g => writeTo(implPath(g))(implFile(g)))
  }

}
