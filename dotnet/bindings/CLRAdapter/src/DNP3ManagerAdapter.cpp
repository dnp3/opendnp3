
#include "DNP3ManagerAdapter.h"

#include "Conversions.h"
#include "LogAdapter.h"
#include "ChannelAdapter.h"

#include <asiodnp3/DNP3Manager.h>

using namespace asiopal;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			IDNP3Manager^ DNP3ManagerFactory::CreateManager(System::Int32 concurrency, ILogHandler^ logHandler)
			{
				return gcnew DNP3ManagerAdapter(concurrency, logHandler);
			}

			IDNP3Manager^ DNP3ManagerFactory::CreateManager(ILogHandler^ logHandler)
			{
				return gcnew DNP3ManagerAdapter(Environment::ProcessorCount, logHandler);
			}


			DNP3ManagerAdapter::DNP3ManagerAdapter(System::Int32 concurrency, ILogHandler^ logHandler) :
				pManager(new asiodnp3::DNP3Manager(concurrency, LogAdapter::Create(logHandler)))				
			{

			}

			DNP3ManagerAdapter::~DNP3ManagerAdapter()
			{
				delete pManager;				
			}

			void DNP3ManagerAdapter::Shutdown()
			{
				pManager->Shutdown();
			}			

			IChannel^ DNP3ManagerAdapter::AddTCPClient(System::String^ id, System::UInt32 filters, ChannelRetry^ retry, System::String^ address, System::UInt16 port)
			{

				std::string stdName = Conversions::ConvertString(id);
				std::string stdAddress = Conversions::ConvertString(address);
				uint16_t stdPort = port;

				auto channel = pManager->AddTCPClient(stdName.c_str(), filters, Conversions::Convert(retry), stdAddress, "", stdPort);
				if (channel)
				{
					auto adapter = gcnew ChannelAdapter(channel);
					channel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));
					return adapter;
				}
				else
				{
					return nullptr;
				}
			}

			IChannel^ DNP3ManagerAdapter::AddTCPServer(System::String^ id, System::UInt32 filters, ChannelRetry^ retry, System::String^ endpoint, System::UInt16 port)
			{
				std::string stdName = Conversions::ConvertString(id);
				std::string stdEndpoint = Conversions::ConvertString(endpoint);
				uint16_t stdPort = port;

				auto channel = pManager->AddTCPServer(stdName.c_str(), filters, Conversions::Convert(retry), stdEndpoint, stdPort);
				if (channel)
				{
					auto adapter = gcnew ChannelAdapter(channel);
					channel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));
					return adapter;
				}
				else
				{
					return nullptr;
				}
			}

			IChannel^ DNP3ManagerAdapter::AddTLSClient(System::String^ id, System::UInt32 filters, ChannelRetry^ retry, System::String^ address, System::UInt16 port, Automatak::DNP3::Interface::TLSConfig^ config)
			{
				std::string stdName = Conversions::ConvertString(id);
				std::string stdAddress = Conversions::ConvertString(address);
				uint16_t stdPort = port;
								
				std::error_code ec;
				auto channel = pManager->AddTLSClient(stdName.c_str(), filters, Conversions::Convert(retry), stdAddress, "", stdPort, Conversions::Convert(config), ec);
				if (ec)
				{
					throw gcnew System::Exception(Conversions::ConvertString(ec.message()));
				}
				else
				{
					auto adapter = gcnew ChannelAdapter(channel);
					channel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));
					return adapter;						
				}								
			}
			
			IChannel^ DNP3ManagerAdapter::AddTLSServer(System::String^ id, System::UInt32 filters, ChannelRetry^ retry, System::String^ endpoint, System::UInt16 port, Automatak::DNP3::Interface::TLSConfig^ config)
			{
				std::string stdName = Conversions::ConvertString(id);
				std::string stdEndpoint = Conversions::ConvertString(endpoint);
				uint16_t stdPort = port;

				
				std::error_code ec;
				auto channel = pManager->AddTLSServer(stdName.c_str(), filters, Conversions::Convert(retry), stdEndpoint, stdPort, Conversions::Convert(config), ec);
				if (ec)
				{
					throw gcnew System::Exception(Conversions::ConvertString(ec.message()));
				}
				else
				{
					auto adapter = gcnew ChannelAdapter(channel);
					channel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));
					return adapter;
				}				
			}

			IChannel^ DNP3ManagerAdapter::AddSerial(System::String^ id, System::UInt32 filters, ChannelRetry^ retry, Automatak::DNP3::Interface::SerialSettings^ settings)
			{
				std::string stdName = Conversions::ConvertString(id);
				auto s = Conversions::ConvertSerialSettings(settings);

				auto channel = pManager->AddSerial(stdName.c_str(), filters, Conversions::Convert(retry), s);
				if (channel)
				{
					auto adapter = gcnew ChannelAdapter(channel);
					channel->DeleteOnDestruct(new gcroot<ChannelAdapter^>(adapter));
					return adapter;
				}
				else
				{
					return nullptr;
				}
			}		

		}
	}
}