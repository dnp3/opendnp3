package com.automatak.dnp3.impl

import org.scalatest.FunSuite
import org.scalatest.junit.JUnitRunner
import org.junit.runner.RunWith
import org.scalatest.matchers.ShouldMatchers
import com.automatak.dnp3._
import com.automatak.dnp3.example.{ConstantCommandHandler, PrintingDataObserver, OutputHandler}

@RunWith(classOf[JUnitRunner])
class DNP3ManagerTestSuite extends FunSuite with ShouldMatchers {

  val iterations = 100

  def fixture(test: DNP3Manager => Unit): Unit = {

      for(i <- (0 to iterations)) {
        val mgr = DNP3ManagerFactory.createDNP3ManagerWithDefaultConcurrency()
        try {
          test(mgr)
        }
        finally {
          mgr.shutdown()
        }
      }

  }

  object SuccessCommandHandler extends ConstantCommandHandler(CommandStatus.SUCCESS)

  object NullDataObserver extends PrintingDataObserver(new OutputHandler {
    def handleOutput(msg: String) = {}
  })

  def createClient(mgr: DNP3Manager): Channel = mgr.addTCPClient("client", LogLevel.INFO, 3000, "127.0.0.1", 20000)
  def createServer(mgr: DNP3Manager): Channel = mgr.addTCPServer("server", LogLevel.INFO, 3000, "127.0.0.1", 20000)
  def createMaster(channel: Channel) : Master = channel.addMaster("master", LogLevel.INFO, NullDataObserver, new MasterStackConfig())
  def createOutstation(channel: Channel): Outstation = channel.addOutstation("outstation", LogLevel.INFO, SuccessCommandHandler)



  test("starts/stops cleanly") {
    fixture { mgr =>

    }
  }

  test("Can add outstation/master and shutdown automatically") {
    fixture { mgr =>
      createOutstation(createServer(mgr))
      createMaster(createClient(mgr))
    }
  }

  test("Can add outstation/master and shutdown manually by removing channels") {
    fixture { mgr =>
      val server = createServer(mgr)
      val client = createClient(mgr)
      createOutstation(server)
      createMaster(client)
      client.shutdown()
      server.shutdown()
    }
  }

  test("Can add outstation/master and shutdown manually by removing stacks") {
    fixture { mgr =>
      val outstation = createOutstation(createServer(mgr))
      val master = createMaster(createClient(mgr))
      outstation.shutdown()
      master.shutdown()
    }
  }


}
