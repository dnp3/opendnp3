package com.automatak.render.cpp

import com.automatak.render._

object ReturnSwitchModelRenderer {

  def wrap[A](f: A => String): A => String = { (a: A) => "return " + f(a) + ";" }

}

class ReturnSwitchModelRenderer[A](fCase: A => String)(fAction: A => String) extends SwitchModelRenderer(fCase)(ReturnSwitchModelRenderer.wrap(fAction))

class SwitchModelRenderer[A](fCase: A => String)(fAction: A => String) extends ModelRenderer[List[A]] {

  def render(a: List[A])(implicit indent: Indentation): Iterator[String] = {

    def switch = Iterator("switch(arg)")

    def cases = a.toIterator.map { c =>
      Iterator(List("case(", fCase(c),"):").mkString) ++
        indent {
          Iterator(List(fAction(c)).mkString)
        }
    }.flatten.toIterator

    switch ++ bracket {
      cases
    }
  }
}
