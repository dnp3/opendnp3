#include <boost/test/unit_test.hpp>

#include <opendnp3/DNP3Manager.h>
#include <opendnp3/IChannel.h>
#include <opendnp3/IMaster.h>
#include <opendnp3/IOutstation.h>
#include <opendnp3/SimpleCommandHandler.h>
#include <opendnp3/IMeasurementHandler.h>
#include <opendnp3/ITimeWriteHandler.h>

#include <asiopal/Log.h>
#include <asiopal/LogToStdio.h>
#include <asiopal/IOServiceThreadPool.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>


#include <asiopal/UTCTimeSource.h>


#include "TestHelpers.h"


#include <thread>

using namespace opendnp3;
using namespace asiopal;

BOOST_AUTO_TEST_SUITE(DNP3ManagerTestSuite)

const size_t ITERATIONS = 100;

BOOST_AUTO_TEST_CASE(ConstructionDestruction)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(Logger(&log, LogLevel::Info, "server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(Logger(&log, LogLevel::Info, "serverChannel"), TimeDuration::Seconds(5), pServerPhys);

		pClient->AddMaster("master", LogLevel::Info, NullMeasurementHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		pServer->AddOutstation("outstation", LogLevel::Info, SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig());
	}
}

BOOST_AUTO_TEST_CASE(ManualStackShutdown)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pClient = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), pClientPhys);

		auto pServerPhys = new PhysicalLayerAsyncTCPServer(Logger(&log, LogLevel::Info, "server"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pServer = mgr.CreateChannel(Logger(&log, LogLevel::Info, "serverChannel"), TimeDuration::Seconds(5), pServerPhys);

		auto pOutstation = pServer->AddOutstation("outstation", LogLevel::Info, SuccessCommandHandler::Inst(), NullTimeWriteHandler::Inst(), SlaveStackConfig());
		auto pMaster = pClient->AddMaster("master", LogLevel::Info, NullMeasurementHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());

		pOutstation->Shutdown();
		pMaster->Shutdown();
	}

}

BOOST_AUTO_TEST_CASE(ManualChannelShutdownWithStack)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		auto pChannel = mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), pClientPhys);

		pChannel->AddMaster("master", LogLevel::Info, NullMeasurementHandler::Inst(), UTCTimeSource::Inst(), MasterStackConfig());
		pChannel->Shutdown();
	}
}

BOOST_AUTO_TEST_CASE(ManualChannelShutdown)
{
	for(int i = 0; i < ITERATIONS; ++i) {

		EventLog log;
		IOServiceThreadPool pool(Logger(&log, LogLevel::Info, "pool"), std::thread::hardware_concurrency());
		DNP3Manager mgr;

		auto pClientPhys = new PhysicalLayerAsyncTCPClient(Logger(&log, LogLevel::Info, "client"), pool.GetIOService(), "127.0.0.1", 20000);
		mgr.CreateChannel(Logger(&log, LogLevel::Info, "clientChannel"), TimeDuration::Seconds(5), pClientPhys)->Shutdown();

	}
}

BOOST_AUTO_TEST_SUITE_END()


