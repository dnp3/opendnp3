
#include "DNP3SessionManagerAdapter.h"

#include "ListenCallbacksAdapter.h"
#include "LogAdapter.h"
#include "Conversions.h"
#include "ListenerAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {
			
	IDNP3SessionManager^ DNP3SessionManagerFactory::CreateManager(System::Int32 concurrency, ILogHandler^ handler)
	{
		return gcnew DNP3SessionManagerAdapter(concurrency, handler);
	}
				
	IDNP3SessionManager^ DNP3SessionManagerFactory::CreateManager(ILogHandler^ handler)
	{
		return CreateManager(Convert::ToUInt32(Environment::ProcessorCount), handler);
	}
					
	DNP3SessionManagerAdapter::DNP3SessionManagerAdapter(System::Int32 concurrency, ILogHandler^ handler) :
		manager(new asiodnp3::GPRSManager(concurrency, LogAdapter::Create(handler)))
	{
	
	}	

	void DNP3SessionManagerAdapter::Shutdown()
	{				
		delete manager;
		manager = nullptr;
	}

	IListener^ DNP3SessionManagerAdapter::CreateListener(System::String^ loggerid, System::UInt32 filters, IPEndpoint^ endpoint, IListenCallbacks^ callbacks)
	{
		auto id = Conversions::ConvertString(loggerid);
		auto levels = openpal::LogFilters(filters);
		auto ep = Conversions::Convert(endpoint);
		auto cb = std::shared_ptr<asiodnp3::IListenCallbacks>(new ListenCallbacksAdapter(callbacks));		

		std::error_code ec;
		auto listener = manager->CreateListener(id, levels, ep, cb, ec);
		
		if (ec)
		{
			throw gcnew System::Exception("An error occured configuring your TCP listener. Check you log.");
		}

		return gcnew ListenerAdapter(listener.get());
	}

	IListener^ DNP3SessionManagerAdapter::CreateListener(System::String^ loggerid, System::UInt32 filters, IPEndpoint^ endpoint, TLSConfig^ config, IListenCallbacks^ callbacks)
	{
		auto id = Conversions::ConvertString(loggerid);
		auto levels = openpal::LogFilters(filters);
		auto ep = Conversions::Convert(endpoint);
		auto tlsConfig = Conversions::Convert(config);
		auto cb = std::shared_ptr<asiodnp3::IListenCallbacks>(new ListenCallbacksAdapter(callbacks));

		std::error_code ec;
		auto listener = manager->CreateListener(id, levels, ep, tlsConfig, cb, ec);

		if (ec)
		{
			throw gcnew System::Exception("An error occured configuring your TLS listener. Check you log.");
		}

		return gcnew ListenerAdapter(listener.get());
	}


}}}


