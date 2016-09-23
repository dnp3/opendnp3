package com.automatak.dnp3

import java.nio.charset.Charset
import java.nio.file.{Files, Path, StandardOpenOption}

package object codegen {

  implicit class RichClass(c: Class[_]) {
    def fqcn : String = "L%s;".format(c.getCanonicalName.replace('.','/'))
  }

  implicit class RichString(s: String) {
    def iter : Iterator[String] = Iterator(s)
  }

  def space : Iterator[String] = "".iter

  def bracketWithCap(cap: String)(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = {
    "{".iter ++
     indent(inner) ++
     "}%s".format(cap).iter
  }

  def bracket(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = bracketWithCap("")(inner)

  def namespace(ns: String)(internals: Iterator[String])(implicit indent: Indentation): Iterator[String] = {
    "namespace %s".format(ns).iter ++ bracket {
      internals
    }
  }

  def commented(lines: Iterator[String]): Iterator[String] = {
    "//".iter ++ lines.map(l => "// " + l) ++ "//".iter
  }

  private def opendnp3Pattern(name: String) = "OPENDNP3JAVA_"+ name.toUpperCase + "_H"

  def includeGuards(name: String)(internals: => Iterator[String]): Iterator[String] = {

    Iterator("#ifndef %s".format(opendnp3Pattern(name))) ++
      Iterator("#define %s".format(opendnp3Pattern(name))) ++
      space ++
      internals ++
      space ++
      Iterator("#endif")

  }

  def writeTo(path: Path)(lines: Iterator[String]): Unit = {

    if(!lines.isEmpty)
    {
      if(!Files.exists(path.getParent)) Files.createDirectory(path.getParent)
      if(!Files.exists(path)) Files.createFile(path)

      val writer = Files.newBufferedWriter( path, Charset.defaultCharset, StandardOpenOption.TRUNCATE_EXISTING)

      def writeLine(s: String) = {
        writer.write(s)
        writer.write(System.lineSeparator)
      }

      try { lines.foreach(writeLine) }
      finally {
        writer.close()
        println("Wrote: " + path.toString)
      }
    }



  }
}
