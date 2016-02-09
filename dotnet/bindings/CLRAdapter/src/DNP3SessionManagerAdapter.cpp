
#include "DNP3SessionManagerAdapter.h"

#include "LogAdapter.h"

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
		delete manager;
	}

	void DNP3SessionManagerAdapter::BeginShutdown()
	{
		manager->BeginShutdown();
	}


	IListener^ DNP3SessionManagerAdapter::CreateListener(System::String^ loggerid, LogFilter filters, IPEndpoint^ endpoint, IListenCallbacks^ callbacks)
	{
		return nullptr;
	}

}}}


