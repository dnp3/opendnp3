#ifndef CLR_DNP3_LISTENER_ADAPTER_H
#define	CLR_DNP3_LISTENER_ADAPTER_H

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

using namespace Automatak::DNP3::Interface;

#include <asiopal/IListener.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class ListenerAdapter sealed : IListener
			{
			public:

				ListenerAdapter(asiopal::IListener* proxy);				

				virtual void BeginShutdown();

			private:
				
				asiopal::IListener* m_proxy;
			};

		}
	}
}

#endif
