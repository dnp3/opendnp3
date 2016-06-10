#include "ListenerAdapter.h"

namespace Automatak {
	namespace DNP3 {
		namespace Adapter {

			ListenerAdapter::ListenerAdapter(asiopal::IListener* proxy)
				: m_proxy(proxy)
			{
			
			}			

			void ListenerAdapter::BeginShutdown()
			{
				if (m_proxy)
				{
					m_proxy->BeginShutdown();
					m_proxy = nullptr;
				}				
			}
		}
	}
}
