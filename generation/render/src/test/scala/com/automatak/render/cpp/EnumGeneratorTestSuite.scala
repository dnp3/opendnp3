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
package com.automatak.render.cpp

import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner
import org.junit.runner.RunWith
import org.scalatest.matchers.ShouldMatchers

import com.automatak.render.{EnumValues, EnumValue, EnumModel}

@RunWith(classOf[JUnitRunner])
class EnumGeneratorTestSuite extends FunSuite with ShouldMatchers {

  test("Simple output") {


     val values = EnumValue("blue", 1, Some("foo")) :: EnumValues.from(List("red", "green", "blue"), 2)
     val eg = EnumModel("Colors", EnumModel.UInt8, values, values.last)

     def lines =  new EnumModelRenderer(CppIndentation()).render(eg)

     lines.toList should equal(
     List(  "enum class Colors : uint8_t",
            "{",
            "  /// foo",
            "  blue = 1,",
            "  red = 2,",
            "  green = 3,",
            "  blue = 4",
            "};"
     ))


  }

}
