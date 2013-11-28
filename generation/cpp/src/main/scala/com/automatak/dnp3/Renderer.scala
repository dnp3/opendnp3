package com.automatak.dnp3

trait Lines extends Traversable[String]

/**
 * Converts some type into a traversable set of lines
 * @tparam A
 */
trait Renderer[A] {

  def apply(a: A): Lines

}
