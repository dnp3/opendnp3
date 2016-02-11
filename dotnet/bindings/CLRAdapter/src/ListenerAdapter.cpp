#include "ListenerAdapter.h"

namespace Automatak {
	namespace DNP3 {
		namespace Adapter {

			ListenerAdapter::ListenerAdapter(std::shared_ptr<asiopal::IListener> proxy)
				: m_proxy(new std::shared_ptr<asiopal::IListener>(proxy))
			{
			
			}

			ListenerAdapter::~ListenerAdapter()
			{
				this->!ListenerAdapter();
			}
			
			ListenerAdapter::!ListenerAdapter()
			{
				delete m_proxy;
			}

			void ListenerAdapter::BeginShutdown()
			{
				(*m_proxy)->BeginShutdown();
			}
		}
	}
}
