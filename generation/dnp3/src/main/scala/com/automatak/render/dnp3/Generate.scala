package com.automatak.render.dnp3

import java.nio.file.FileSystems
import com.automatak.render.dnp3.enums.generators.{CSharpEnumGenerator, CppEnumGenerator}
import com.automatak.render.dnp3.enums.groups.{CSharpEnumGroup, DNPCppEnumGroup}
import com.automatak.render.dnp3.objects.generators.GroupVariationFileGenerator

object Generate {

  val dnp3GenHeaderPath = FileSystems.getDefault.getPath("../cpp/libs/include/opendnp3/gen")
  val dnp3GenImplPath = FileSystems.getDefault.getPath("../cpp/libs/src/opendnp3/gen")

  val dnp3ObjectPath = FileSystems.getDefault.getPath("../cpp/libs/src/opendnp3/objects")

  val csharpGenPath = FileSystems.getDefault.getPath("../dotnet/bindings/CLRInterface/gen")

  def main(args: Array[String]): Unit = {

    // generate all the required filter items
    CppEnumGenerator(DNPCppEnumGroup.enums, "opendnp3", dnp3GenHeaderPath, dnp3GenImplPath)

    CSharpEnumGenerator(CSharpEnumGroup.enums, "Automatak.DNP3.Interface", csharpGenPath)


    GroupVariationFileGenerator(dnp3ObjectPath)
  }

}
