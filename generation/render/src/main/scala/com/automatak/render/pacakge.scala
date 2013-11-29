/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
package com.automatak

import java.nio.file.{Files, StandardOpenOption, Path}
import java.nio.charset.Charset

package object render {

    def space: Iterator[String] = Iterator.apply("")

    def bracket[A](indent: Indentation, cap: String = "")(lines: String => A)(inner: => Unit): Unit = {
      lines(indent.wrap("{"))
      indent {
        inner
      }
      lines(indent.wrap("}"+cap))
    }

    def bracketSemiColon[A](indent: Indentation)(lines: String => A)(inner: => Unit): Unit = bracket(indent,";")(lines)(inner)

    implicit class RichStringList(list: List[String]) {

      def spaced : String = list.mkString(" ")

      def commaDelimited: List[String] = list.sliding(2).map(x => x.head + ",").toList ::: List(list.last)

    }

    def writeLinesTo(path: Path, lines: Iterator[String]): Unit = {

      val writer = Files.newBufferedWriter( path, Charset.defaultCharset, StandardOpenOption.TRUNCATE_EXISTING)

      def writeLine(s: String) = {
        writer.write(s)
        writer.write(System.lineSeparator)
      }

      try { lines.foreach(writeLine) }
      finally { writer.close() }

    }




}
