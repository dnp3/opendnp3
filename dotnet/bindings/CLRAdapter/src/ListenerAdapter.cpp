#include "ListenerAdapter.h"

namespace Automatak {
	namespace DNP3 {
		namespace Adapter {

			ListenerAdapter::ListenerAdapter(asiopal::IListener* proxy): proxy(proxy)
			{
			
			}			

			void ListenerAdapter::BeginShutdown()
			{
				if (proxy)
				{
					proxy->BeginShutdown();
					proxy = nullptr;
				}				
			}
		}
	}
}
