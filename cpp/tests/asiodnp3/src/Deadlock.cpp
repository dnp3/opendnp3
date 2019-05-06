#include <catch.hpp>
#include <thread>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DefaultMasterApplication.h>
#include <asiodnp3/PrintingCommandCallback.h>
#include <asiodnp3/PrintingChannelListener.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <asiopal/UTCTimeSource.h>
#include <opendnp3/LogLevels.h>

using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace opendnp3;

const uint32_t FILTERS = levels::NOTHING;
const uint32_t NUM_THREADS = 1;

void start_outstation(DNP3Manager& manager)
{
	const auto channel = manager.AddTCPServer("server", FILTERS, ServerAcceptMode::CloseNew, "127.0.0.1", 20000, nullptr);
	const auto outstation = channel->AddOutstation("outstation", SuccessCommandHandler::Create(), DefaultOutstationApplication::Create(), OutstationStackConfig(DatabaseSizes::AllTypes(0)));
	outstation->Enable();
}

void start_master(DNP3Manager& manager)
{
	const auto channel = manager.AddTCPClient("client", FILTERS, ChannelRetry(TimeDuration::Seconds(0), TimeDuration::Seconds(0)), "127.0.0.1", "127.0.0.1", 20000, nullptr);
	const auto master = channel->AddMaster("master", PrintingSOEHandler::Create(), asiodnp3::DefaultMasterApplication::Create(), MasterStackConfig());
	const auto scan = master->AddClassScan(ClassField::AllClasses(), TimeDuration::Milliseconds(1));
	master->Enable();
}

TEST_CASE("TestDeadlock")
{
	for(int i = 0; i < 1000; ++i)
	{
		if(i % 100 == 0)
		{
			std::cout << "start iteration: " << i << std::endl;
		}

		DNP3Manager manager2(NUM_THREADS);
		DNP3Manager manager1(NUM_THREADS);

		const bool is_even = (i % 2) == 0;

		start_outstation(is_even  ? manager1 : manager2);
		start_master(is_even  ? manager2 : manager1);

		// give the two sides sufficient time to connect
		std::this_thread::sleep_for(std::chrono::milliseconds(5));

		// shutdown manager2 followed by manager1
	}
}
