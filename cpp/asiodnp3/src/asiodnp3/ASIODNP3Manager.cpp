
#include <asiodnp3/ASIODNP3Manager.h>

#include <asiopal/Log.h>
#include <asiopal/IOServiceThreadPool.h>
#include <opendnp3/DNP3Manager.h>

#include <asiopal/PhysicalLayerAsyncSerial.h>
#include <asiopal/PhysicalLayerAsyncTCPClient.h>
#include <asiopal/PhysicalLayerAsyncTCPServer.h>

using namespace openpal;

namespace asiodnp3
{


ASIODNP3Manager::ASIODNP3Manager(uint32_t aConcurrency, std::function<void()> aOnThreadStart, std::function<void()> aOnThreadExit) :
	DestructorHook(nullptr),
	mpLog(new asiopal::EventLog()),
	mpThreadPool(new asiopal::IOServiceThreadPool(Logger(mpLog.get(), LEV_INFO, "pool"), aConcurrency, aOnThreadStart, aOnThreadExit)),
	mpManager(new opendnp3::DNP3Manager())
{

}

ASIODNP3Manager::~ASIODNP3Manager()
{

}

void ASIODNP3Manager::AddLogSubscriber(openpal::ILogBase* apLog)
{
	mpLog->AddLogSubscriber(apLog);
}

openpal::ILogBase* ASIODNP3Manager::GetLog()
{
	return mpLog.get();
}

void ASIODNP3Manager::Shutdown()
{
	mpManager->Shutdown();
}

opendnp3::IChannel* ASIODNP3Manager::AddTCPClient(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, const std::string& arHost, uint16_t aPort)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPClient(aLogger, mpThreadPool->GetIOService(), arHost, aPort);
	return mpManager->CreateChannel(aLogger, aOpenRetry, pPhys);
}

opendnp3::IChannel* ASIODNP3Manager::AddTCPServer(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, const std::string& arEndpoint, uint16_t aPort)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncTCPServer(aLogger, mpThreadPool->GetIOService(), arEndpoint, aPort);
	return mpManager->CreateChannel(aLogger, aOpenRetry, pPhys);
}

opendnp3::IChannel* ASIODNP3Manager::AddSerial(openpal::Logger aLogger, openpal::TimeDuration aOpenRetry, asiopal::SerialSettings aSettings)
{
	auto pPhys = new asiopal::PhysicalLayerAsyncSerial(aLogger, mpThreadPool->GetIOService(), aSettings);
	return mpManager->CreateChannel(aLogger, aOpenRetry, pPhys);
}

}

