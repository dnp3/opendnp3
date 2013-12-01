package com.automatak.render

package object csharp {

  def namespace(namespace: List[String])(inner: => Iterator[String])(implicit indent: Indentation): Iterator[String] = {
    Iterator("namespace " + namespace.mkString(".")) ++ bracket {
      inner
    }
  }
}
