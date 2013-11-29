package com.automatak.render

trait Indentation {

  def apply(f: => Unit): Unit

  def wrap(s: String): String

}
