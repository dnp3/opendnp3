#ifndef OPENDNP3CLR_CHANNEL_ADAPTER_H
#define OPENDNP3CLR_CHANNEL_ADAPTER_H

#include <asiodnp3/IChannel.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			ref class ChannelAdapter : IChannel
			{
			public:

				ChannelAdapter(const std::shared_ptr<asiodnp3::IChannel>& channel);

				~ChannelAdapter();
				!ChannelAdapter();

				virtual LogFilter GetLogFilters() sealed;

				virtual IChannelStatistics^ GetChannelStatistics() sealed;

				virtual void SetLogFilters(LogFilter filters) sealed;				

				virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, IMasterApplication^ application, MasterStackConfig^ config) sealed;

				virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config) sealed;

				virtual void Shutdown() sealed;

			private:

				std::shared_ptr<asiodnp3::IChannel>* channel;
			};

		}
	}
}

#endif
