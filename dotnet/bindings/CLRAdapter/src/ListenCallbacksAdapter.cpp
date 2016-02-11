
#include "ListenCallbacksAdapter.h"

#include "Conversions.h"

namespace Automatak { namespace DNP3 { namespace Adapter {

	bool ListenCallbacksAdapter::AcceptConnection(const std::string& ipaddress)
	{		
		return proxy->AcceptConnection(Conversions::ConvertString(ipaddress));
	}

	openpal::TimeDuration ListenCallbacksAdapter::GetFirstFrameTimeout()
	{
		return Conversions::ConvertTimespan(proxy->GetFirstFrameTimeout());
	}

	void ListenCallbacksAdapter::OnFirstFrame(const opendnp3::LinkHeaderFields& header, asiodnp3::ISessionAcceptor& acceptor)
	{
		auto linkheader = Conversions::Convert(header);
	}

	void ListenCallbacksAdapter::OnSessionClose(std::shared_ptr<asiodnp3::IGPRSMaster> session)
	{
	
	}

}}}


