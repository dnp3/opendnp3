package com.automatak.render.dnp3.objects.generators

import java.nio.file.Path
import com.automatak.render.dnp3.objects.ObjectGroup
import com.automatak.render._
import com.automatak.render.cpp._
import com.automatak.render.LicenseHeader

object GroupVariationFileGenerator {

  def apply(path: Path) = {

    implicit val indent = CppIndentation()

    def headerPath(group: ObjectGroup): Path = path.resolve("Group"+group.group+".h")
    def implPath(group: ObjectGroup): Path = path.resolve("Group"+group.group+".cpp")

    def classDefinitions(group: ObjectGroup): Iterator[String] = spaced(group.objects.iterator.map(o => GroupVariationHeaderRenderer.render(o)))

    def methodDefinitions(group: ObjectGroup): Iterator[String] = spaced(group.objects.iterator.map(o => GroupVariationImplRenderer.render(o)))

    def headerFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
      namespace("opendnp3") {
        classDefinitions(group)
      }
    }

    def implFile(group: ObjectGroup): Iterator[String] = {
      commented(LicenseHeader()) ++ space ++
        namespace("opendnp3") {
          methodDefinitions(group)
        }
    }

    ObjectGroup.all.foreach(g => writeTo(headerPath(g))(headerFile(g)))
    ObjectGroup.all.foreach(g => writeTo(implPath(g))(implFile(g)))
  }

}
