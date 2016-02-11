
#include "ListenCallbacksAdapter.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

	bool ListenCallbacksAdapter::AcceptConnection(const std::string& ipaddress)
	{
		return false;
	}

	openpal::TimeDuration ListenCallbacksAdapter::GetFirstFrameTimeout()
	{
		return openpal::TimeDuration::Seconds(1);
	}

	void ListenCallbacksAdapter::OnFirstFrame(const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor)
	{
	
	}

	void ListenCallbacksAdapter::OnSessionClose(std::shared_ptr<asiodnp3::IGPRSMaster> session)
	{
	
	}

}}}


