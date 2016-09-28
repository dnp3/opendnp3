
#include "ChannelAdapter.h"

#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "OutstationApplicationAdapter.h"
#include "MasterApplicationAdapter.h"

#include "OutstationAdapter.h"
#include "MasterAdapter.h"

#include "EventConverter.h"

#include <asiopal/UTCTimeSource.h>
#include <functional>

using namespace System::Collections::Generic;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			LogFilter ChannelAdapter::GetLogFilters()
			{
				return LogFilter(pChannel->GetLogFilters().GetBitfield());
			}

			IChannelStatistics^ ChannelAdapter::GetChannelStatistics()
			{
				auto stats = pChannel->GetChannelStatistics();
				return Conversions::ConvertChannelStats(stats);
			}

			void ChannelAdapter::SetLogFilters(LogFilter filters)
			{
				openpal::LogFilters flags(filters.Flags);
				pChannel->SetLogFilters(flags);
			}

			void ChannelAdapter::AddStateListener(System::Action<ChannelState>^ listener)
			{
				auto convert = std::bind(&Conversions::ConvertChannelState, std::placeholders::_1);
				auto pConverter = new EventConverter<opendnp3::ChannelState, Automatak::DNP3::Interface::ChannelState>(convert, listener);
				std::function<void(opendnp3::ChannelState)> trigger = pConverter->GetTrigger();				
				pChannel->DeleteOnDestruct(pConverter);
			}

			void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState aState)
			{
				ChannelState state = Conversions::ConvertChannelState(aState);
				(*listener)->Invoke(state);
			}

			IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ handler, IMasterApplication^ application, MasterStackConfig^ config)
			{
				std::string stdLoggerId = Conversions::ConvertString(loggerId);

				asiodnp3::MasterStackConfig cfg(Conversions::ConvertConfig(config));

				auto SOEAdapter = std::shared_ptr<opendnp3::ISOEHandler>(new SOEHandlerAdapter(handler));
				auto appAdapter = std::shared_ptr<opendnp3::IMasterApplication>(new MasterApplicationAdapter<opendnp3::IMasterApplication>(application));

				auto master = pChannel->AddMaster(stdLoggerId.c_str(), SOEAdapter, appAdapter, cfg);
				return (master == nullptr) ? nullptr : gcnew MasterAdapter(master);
			}	

			IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config)
			{
				std::string stdLoggerId = Conversions::ConvertString(loggerId);

				asiodnp3::OutstationStackConfig cfg = Conversions::ConvertConfig(config);

				auto commandAdapter = std::shared_ptr<opendnp3::ICommandHandler>(new OutstationCommandHandlerAdapter(cmdHandler));
				auto appAdapter = std::shared_ptr<opendnp3::IOutstationApplication>(new OutstationApplicationAdapter(application));

				auto outstation = pChannel->AddOutstation(stdLoggerId.c_str(), commandAdapter, appAdapter, Conversions::ConvertConfig(config));
				return outstation ? gcnew OutstationAdapter(outstation) : nullptr;
			}			

			void ChannelAdapter::Shutdown()
			{
				pChannel->Shutdown();
			}

		}
	}
}
