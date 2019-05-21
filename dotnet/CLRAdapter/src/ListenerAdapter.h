#ifndef OPENDNP3CLR_LISTENER_ADAPTER_H
#define	OPENDNP3CLR_LISTENER_ADAPTER_H

#include <asiopal/IListener.h>

using namespace Automatak::DNP3::Interface;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Threading::Tasks;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private ref class ListenerAdapter sealed : Interface::IListener
			{
			public:

				ListenerAdapter(const std::shared_ptr<asiopal::IListener>& proxy) : proxy(new std::shared_ptr<asiopal::IListener>(proxy))
				{}

				ListenerAdapter::~ListenerAdapter()
				{
					this->!ListenerAdapter();
				}

				ListenerAdapter::!ListenerAdapter()
				{
					delete proxy;
				}

				virtual void BeginShutdown()
				{
					(*proxy)->Shutdown();
				}

			private:
				
				const std::shared_ptr<asiopal::IListener>* proxy;
			};

		}
	}
}

#endif
