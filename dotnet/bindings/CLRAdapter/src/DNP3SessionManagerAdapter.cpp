
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
		manager(new asiodnp3::GPRSManager(concurrency, (gcnew LogAdapterWrapper(handler))->GetLogAdapter()))
	{}
	
	DNP3SessionManagerAdapter::~DNP3SessionManagerAdapter()
	{
		this->!DNP3SessionManagerAdapter();
	}

	DNP3SessionManagerAdapter::!DNP3SessionManagerAdapter()
	{
		delete manager;
	}

	void DNP3SessionManagerAdapter::BeginShutdown()
	{		
		manager->BeginShutdown();
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
			throw gcnew System::Exception(Conversions::ConvertString(ec.message()));
		}

		return gcnew ListenerAdapter(listener);
	}

}}}


