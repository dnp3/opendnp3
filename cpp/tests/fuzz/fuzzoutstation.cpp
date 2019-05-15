#include <opendnp3/LogLevels.h>
#include <opendnp3/app/ITransactable.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/outstation/OutstationContext.h>

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

#include <dnp3mocks/MockCommandHandler.h>
#include <dnp3mocks/MockLowerLayer.h>
#include <dnp3mocks/MockOutstationApplication.h>

#include <functional>

class OutstationTestObject
{
public:
	OutstationTestObject(const opendnp3::OutstationConfig& config, const opendnp3::DatabaseSizes& dbSizes = opendnp3::DatabaseSizes::Empty())
      : log(),
        exe(std::make_shared<testlib::MockExecutor>()),
        lower(std::make_shared<opendnp3::MockLowerLayer>()),
        cmdHandler(std::make_shared<opendnp3::MockCommandHandler>(opendnp3::CommandStatus::SUCCESS)),
        application(std::make_shared<opendnp3::MockOutstationApplication>()),
        context(opendnp3::Addresses(), config, dbSizes, log.logger, exe, lower, cmdHandler, application)
    {
        lower->SetUpperLayer(context);
    }

	size_t SendToOutstation(const openpal::RSlice& buffer)
    {
        context.OnReceive(opendnp3::Message(opendnp3::Addresses(), buffer));
	    return exe->RunMany();
    }

	size_t LowerLayerUp()
    {
        context.OnLowerLayerUp();
		return exe->RunMany();
    }

private:
    testlib::MockLogHandler log;
    const std::shared_ptr<testlib::MockExecutor> exe;
	const std::shared_ptr<opendnp3::MockLowerLayer> lower;
	const std::shared_ptr<opendnp3::MockCommandHandler> cmdHandler;
	const std::shared_ptr<opendnp3::MockOutstationApplication> application;
	opendnp3::OContext context;
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    openpal::RSlice buffer(Data, Size);

    opendnp3::OutstationConfig config;
    OutstationTestObject t(config);
    t.LowerLayerUp();
    t.SendToOutstation(buffer);
    return 0;
}
