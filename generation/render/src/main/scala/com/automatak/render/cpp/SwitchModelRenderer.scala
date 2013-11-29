package com.automatak.render.cpp

import com.automatak.render._

object ReturnSwitchModelRenderer {

  def wrap[A](f: A => String): A => String = { (a: A) => "return " + f(a) + ";" }

}

class ReturnSwitchModelRenderer[A](indent: Indentation)(fCase: A => String)(fAction: A => String)
  extends SwitchModelRenderer(indent)(fCase)(ReturnSwitchModelRenderer.wrap(fAction))

class SwitchModelRenderer[A](indent: Indentation)(fCase: A => String)(fAction: A => String)
  extends ModelRenderer[List[A]] {

  def render(a: List[A]): Iterator[String] = {

    def switch = Iterator("switch(arg)")

    def cases = a.toIterator.map { c =>
      Iterator(List("case(", fCase(c),"):").mkString) ++
        indent {
          Iterator(List(fAction(c)).mkString)
        }
    }.flatten.toIterator

    switch ++ bracket(indent) {
      cases
    }
  }
}
