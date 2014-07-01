#include "DNP3ManagerAdapter.h"

#include "Conversions.h"
#include "LogAdapter.h"
#include "ChannelAdapter.h"
#include "DeleteAnything.h"

#include <asiodnp3/DNP3Manager.h>

using namespace asiopal;

namespace DNP3
{
namespace Adapter
{

IDNP3Manager^ DNP3ManagerFactory::CreateManager(System::Int32 aConcurrency)
{
	return gcnew DNP3ManagerAdapter(aConcurrency);
}

IDNP3Manager^ DNP3ManagerFactory::CreateManager()
{
	return gcnew DNP3ManagerAdapter(Environment::ProcessorCount);
}


DNP3ManagerAdapter::DNP3ManagerAdapter(System::Int32 aConcurrency) :
	mpMgr(new asiodnp3::DNP3Manager(aConcurrency))
{

}

DNP3ManagerAdapter::~DNP3ManagerAdapter()
{
	delete mpMgr;
}

IChannel^ DNP3ManagerAdapter::AddTCPClient(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ address, System::UInt16 port)
{

	std::string stdName = Conversions::ConvertString(id);
	std::string stdAddress = Conversions::ConvertString(address);
	uint16_t stdPort = port;

	auto pChannel = mpMgr->AddTCPClient(stdName.c_str(), filters, Conversions::ConvertTimespan(minRetryDelay), Conversions::ConvertTimespan(maxRetryDelay), stdAddress, stdPort);
	if (pChannel)
	{		
		auto adapter = gcnew ChannelAdapter(pChannel);
		pChannel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));		
		return adapter;
	}
	else
	{
		return nullptr;
	}
}

IChannel^ DNP3ManagerAdapter::AddTCPServer(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ endpoint, System::UInt16 port)
{
	std::string stdName = Conversions::ConvertString(id);
	std::string stdEndpoint = Conversions::ConvertString(endpoint);
	uint16_t stdPort = port;
	
	auto pChannel = mpMgr->AddTCPServer(stdName.c_str(), filters, Conversions::ConvertTimespan(minRetryDelay), Conversions::ConvertTimespan(maxRetryDelay), stdEndpoint, stdPort);
	if (pChannel)
	{
		auto adapter = gcnew ChannelAdapter(pChannel);
		pChannel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));		
		return adapter;
	}
	else
	{
		return nullptr;
	}
}

IChannel^ DNP3ManagerAdapter::AddSerial(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, DNP3::Interface::SerialSettings^ settings)
{
	std::string stdName = Conversions::ConvertString(id);
	auto s = Conversions::ConvertSerialSettings(settings);
	
	auto pChannel = mpMgr->AddSerial(stdName.c_str(), filters, Conversions::ConvertTimespan(minRetryDelay), Conversions::ConvertTimespan(maxRetryDelay), s);
	if (pChannel)
	{
		auto adapter = gcnew ChannelAdapter(pChannel);
		pChannel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));		
		return adapter;
	}
	else
	{
		return nullptr;
	}
}

void DNP3ManagerAdapter::AddLogHandler(ILogHandler^ logHandler)
{
	LogAdapterWrapper^ wrapper = gcnew LogAdapterWrapper(logHandler);
	mpMgr->AddLogSubscriber(wrapper->GetLogAdapter());
}

}
}
