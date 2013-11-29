package com.automatak.render.cpp

import com.automatak.render.Indentation

object CppIndentation {
  def apply(): Indentation = IndentationImpl("  ")
}


private case class IndentationImpl(indent: String) extends Indentation {

  private var count = 0

  def apply(f: => Unit): Unit = {
    count += 1
    f
    count -= 1
  }

  def wrap(s: String): String = Iterator.fill(count)(indent).mkString("") + s

}
