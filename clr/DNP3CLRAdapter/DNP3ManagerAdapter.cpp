#include "DNP3ManagerAdapter.h"

#include "Conversions.h"
#include "LogAdapter.h"
#include "ChannelAdapter.h"
#include "DeleteAnything.h"

#include <asiodnp3/ASIODNP3Manager.h>

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
	mpMgr(new asiodnp3::ASIODNP3Manager(aConcurrency))
{

}

DNP3ManagerAdapter::~DNP3ManagerAdapter()
{
	delete mpMgr;
}

IChannel^ DNP3ManagerAdapter::AddTCPClient(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ address, System::UInt16 port)
{

	std::string stdName = Conversions::convertString(id);
	std::string stdAddress = Conversions::convertString(address);
	uint16_t stdPort = port;	

	auto adapter = gcnew ChannelAdapter();
	
	auto pChannel = mpMgr->AddTCPClient(stdName, filters, Conversions::convertTimespan(minRetryDelay), Conversions::convertTimespan(maxRetryDelay), stdAddress, stdPort, adapter->GetEventHandler());
	if (pChannel)
	{
		auto pRoot = new gcroot<ChannelAdapter^>(adapter);
		pChannel->AddDestructorHook(std::bind(&DeleteAnything<gcroot<ChannelAdapter^>>, pRoot));
		adapter->SetChannel(pChannel);
		return adapter;		
	}
	else
	{
		return nullptr;
	}
}

IChannel^ DNP3ManagerAdapter::AddTCPServer(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, System::String^ endpoint, System::UInt16 port)
{
	std::string stdName = Conversions::convertString(id);
	std::string stdEndpoint = Conversions::convertString(endpoint);
	uint16_t stdPort = port;	

	auto adapter = gcnew ChannelAdapter();	
	auto pChannel = mpMgr->AddTCPServer(stdName, filters, Conversions::convertTimespan(minRetryDelay), Conversions::convertTimespan(maxRetryDelay), stdEndpoint, stdPort, adapter->GetEventHandler());
	if (pChannel)
	{
		auto pRoot = new gcroot<ChannelAdapter^>(adapter);
		pChannel->AddDestructorHook(std::bind(&DeleteAnything<gcroot<ChannelAdapter^>>, pRoot));
		adapter->SetChannel(pChannel);
		return adapter;		
	}
	else
	{
		return nullptr;
	}
}

IChannel^ DNP3ManagerAdapter::AddSerial(System::String^ id, System::UInt32 filters, System::TimeSpan minRetryDelay, System::TimeSpan maxRetryDelay, DNP3::Interface::SerialSettings^ settings)
{
	std::string stdName = Conversions::convertString(id);	
	auto s = Conversions::convertSerialSettings(settings);

	auto adapter = gcnew ChannelAdapter();	
	auto pChannel = mpMgr->AddSerial(stdName, filters, Conversions::convertTimespan(minRetryDelay), Conversions::convertTimespan(maxRetryDelay), s, adapter->GetEventHandler());
	if (pChannel)
	{
		auto pRoot = new gcroot<ChannelAdapter^>(adapter);
		pChannel->AddDestructorHook(std::bind(&DeleteAnything<gcroot<ChannelAdapter^>>, pRoot));
		adapter->SetChannel(pChannel);
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
