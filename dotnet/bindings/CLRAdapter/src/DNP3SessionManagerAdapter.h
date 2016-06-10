#ifndef CLR_DNP3_SESSION_MANAGER_ADAPTER_H
#define CLR_DNP3_SESSION_MANAGER_ADAPTER_H

using namespace System;
using namespace Automatak::DNP3::Interface;

#include <asiodnp3/GPRSManager.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			/// <summary>
			/// Factory class used to get the root DNP3 object
			/// </summary>
			public ref class DNP3SessionManagerFactory
			{
			public:
				/// <summary>
				/// Create a new IDNP3Manager using the specified concurrency
				/// </summary>
				/// <param name="concurrency">how many threads are allocated to the thread pool</param>
				/// <returns>IDNP3Manager interface</returns>
				static IDNP3SessionManager^ CreateManager(System::Int32 concurrency, ILogHandler^ handler);

				/// <summary>
				/// Create a new IDNP3Manager using the default concurrency
				/// </summary>
				/// <returns>IDNP3Manager interface</returns>
				static IDNP3SessionManager^ CreateManager(ILogHandler^ handler);

			private:
				DNP3SessionManagerFactory() {}
			};


			ref class DNP3SessionManagerAdapter : public Automatak::DNP3::Interface::IDNP3SessionManager
			{
			public:
				
				DNP3SessionManagerAdapter(System::Int32 concurrency, ILogHandler^ handler);							

				virtual void Shutdown() sealed;

				virtual IListener^ CreateListener(System::String^ loggerid, System::UInt32 filters, IPEndpoint^ endpoint, IListenCallbacks^ callbacks) sealed;	

				virtual IListener^ CreateListener(System::String^ loggerid, System::UInt32 filters, IPEndpoint^ endpoint, TLSConfig^ config, IListenCallbacks^ callbacks) sealed;

			private:
				
				asiodnp3::GPRSManager* manager;								
			};

		}
	}
}

#endif
