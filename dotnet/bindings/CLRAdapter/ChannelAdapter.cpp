
#include "Stdafx.h"
#include "ChannelAdapter.h"

#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "OutstationApplicationAdapter.h"
#include "MasterApplicationAdapter.h"
#include "MasterAdapter.h"
#include "OutstationAdapter.h"
#include "EventConverter.h"

#include <opendnp3/outstation/Database.h>

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
				pChannel->AddStateListener(trigger);
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

				opendnp3::MasterStackConfig cfg = Conversions::ConvertConfig(config);

				auto pSOEHandler = new SOEHandlerAdapter(handler);
				auto pApplication = new MasterApplicationAdapter(application);

				auto pMaster = pChannel->AddMaster(stdLoggerId.c_str(), *pSOEHandler, *pApplication, cfg);
				if (pMaster == nullptr)
				{
					delete pSOEHandler;
					delete pApplication;
					return nullptr;
				}
				else
				{
					pMaster->DeleteOnDestruct(pSOEHandler);
					pMaster->DeleteOnDestruct(pApplication);
					return gcnew MasterAdapter(pMaster);
				}
			}

			IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config)
			{
				std::string stdLoggerId = Conversions::ConvertString(loggerId);

				opendnp3::OutstationStackConfig cfg = Conversions::ConvertConfig(config);

				auto pCommand = new OutstationCommandHandlerAdapter(cmdHandler);
				auto pApplication = new OutstationApplicationAdapter(application);

				auto pOutstation = pChannel->AddOutstation(stdLoggerId.c_str(), *pCommand, *pApplication, Conversions::ConvertConfig(config));
				if (pOutstation == nullptr)
				{
					delete pCommand;
					delete pApplication;
					return nullptr;
				}
				else
				{
					pOutstation->DeleteOnDestruct(pCommand);
					pOutstation->DeleteOnDestruct(pApplication);

					ApplyDatabaseSettings(pOutstation->GetDatabase(), config->databaseTemplate);
					return gcnew OutstationAdapter(pOutstation);
				}
			}

			void ChannelAdapter::ApplyDatabaseSettings(opendnp3::Database& database, DatabaseTemplate^ dbTemplate)
			{
				ApplyClassSettings(dbTemplate->binaries, database.staticData.binaries);
				ApplyClassSettings(dbTemplate->binaryOutputStatii, database.staticData.binaryOutputStatii);
				ApplyClassSettings(dbTemplate->doubleBinaries, database.staticData.doubleBinaries);

				ApplyClassAndDeadbandSettings(dbTemplate->analogs, database.staticData.analogs);
				ApplyClassAndDeadbandSettings(dbTemplate->counters, database.staticData.counters);
				ApplyClassAndDeadbandSettings(dbTemplate->frozenCounters, database.staticData.frozenCounters);
				ApplyClassAndDeadbandSettings(dbTemplate->analogOutputStatii, database.staticData.analogOutputStatii);

			}

			void ChannelAdapter::Shutdown()
			{
				pChannel->Shutdown();
			}

		}
	}
}
