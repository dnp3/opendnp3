package com.automatak.render.dnp3.objects.generators

import java.nio.file.Path
import com.automatak.render.dnp3.objects.ObjectGroup
import com.automatak.render.cpp.CppIndentation

object GroupVariationFileGenerator {

  def apply(path: Path) = {

    implicit val indent = CppIndentation()

    def headerPath(group: ObjectGroup): Path = path.resolve("Group"+group.group+".h")
    def implPath(group: ObjectGroup): Path = path.resolve("Group"+group.group+".cpp")

    def classDefinitions(group: ObjectGroup): Iterator[String] = group.objects.foreach(o => GroupVariationHeaderRenderer.render(o))

    def writeGroupToFiles(group: ObjectGroup): Unit = {

    }

    ObjectGroup.all.foreach(writeGroupToFiles)
  }

}
