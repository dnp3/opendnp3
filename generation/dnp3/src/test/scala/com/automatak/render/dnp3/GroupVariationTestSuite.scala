package com.automatak.render.dnp3

import org.scalatest.FunSuite
import org.scalatest.matchers.ShouldMatchers
import org.scalatest.junit.JUnitRunner
import org.junit.runner.RunWith
import com.automatak.render.dnp3.objects.ObjectGroup
import com.automatak.render.dnp3.objects.groups.Group12Var1

@RunWith(classOf[JUnitRunner])
class GroupVariationTestSuite extends FunSuite with ShouldMatchers {

  test("Object ids are not repeated") {

    val ids = for {
      group <- ObjectGroup.all
      gv <- group.objects
    } yield gv.id

    ids.toSet.size should equal(ids.size)

  }

  test("Fixed size calculated correctly") {
    Group12Var1.size should equal(11)
  }

}
