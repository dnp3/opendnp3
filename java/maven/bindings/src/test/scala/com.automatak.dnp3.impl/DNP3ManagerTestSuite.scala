package com.automatak.dnp3.impl

import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner
import org.junit.runner.RunWith
import org.scalatest.matchers.ShouldMatchers
import com.automatak.dnp3._
import com.automatak.dnp3.example.{ConstantCommandHandler, PrintingDataObserver, OutputHandler}

@RunWith(classOf[JUnitRunner])
class DNP3ManagerTestSuite extends FunSuite with ShouldMatchers {

  def fixture[A](test: DNP3Manager => A): A = {
    val mgr = DNP3ManagerFactory.createDNP3ManagerWithDefaultConcurrency()
    try {
      test(mgr)
    }
    finally {
      mgr.shutdown()
    }
  }

  object SuccessCommandHandler extends ConstantCommandHandler(CommandStatus.SUCCESS)

  object NullDataObserver extends PrintingDataObserver(new OutputHandler {
    def handleOutput(msg: String) = {}
  })

  test("starts/stops cleanly") {
    fixture { mgr =>

    }
  }

  test("Can add outstation/slave and shutdown automatically") {
    fixture { mgr =>
      val client = mgr.addTCPClient("client", LogLevel.INFO, 3000, "127.0.0.1", 20000)
      val server = mgr.addTCPServer("server", LogLevel.INFO, 3000, "127.0.0.1", 20000)
      val outstation = server.addOutstation("outstation", LogLevel.INFO, SuccessCommandHandler)
      val master = client.addMaster("master", LogLevel.INFO, NullDataObserver)
    }
  }


}
