
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

		}
	}
}