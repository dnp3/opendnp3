package com.automatak.render.dnp3

import org.junit.runner.RunWith
import org.scalatest.junit.JUnitRunner
import org.scalatest.FunSuite
import org.scalatest.matchers.ShouldMatchers

import com.automatak.render.dnp3.objects.ObjectGroup
import com.automatak.render.dnp3.objects.generators._
import com.automatak.render.cpp.CppIndentation


@RunWith(classOf[JUnitRunner])
class GroupVariationRenderingTestSuite extends FunSuite with ShouldMatchers {

  test("Experiment with rendering") {

      implicit val indentation = CppIndentation()

      for {
        g <- ObjectGroup.all
        v <- g.objects
      } GroupVariationImplRenderer.render(v).foreach(println)
  }



}
