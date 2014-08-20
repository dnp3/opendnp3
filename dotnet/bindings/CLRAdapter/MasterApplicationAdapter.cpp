
#include "Stdafx.h"
#include "MasterApplicationAdapter.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{


			MasterApplicationAdapter::MasterApplicationAdapter(Automatak::DNP3::Interface::IMasterApplication^ proxy_) : proxy(proxy_)
			{}

			openpal::UTCTimestamp MasterApplicationAdapter::Now()
			{
				auto milliseconds = proxy->GetMillisecondsSinceEpoch();
				return openpal::UTCTimestamp(milliseconds);
			}

			void MasterApplicationAdapter::OnReceiveIIN(const opendnp3::IINField& iin)
			{
				// TODO, add to C# API and translate type
			}

		}
	}
}