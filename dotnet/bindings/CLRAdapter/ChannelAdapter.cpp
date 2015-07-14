
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
#include "MasterUserDatabaseAdapter.h"

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

			IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ handler, IMasterApplication^ application, MasterStackConfig^ config, IMasterUser^ masterUser)
			{
				std::string stdLoggerId = Conversions::ConvertString(loggerId);

				opendnp3::MasterStackConfig cfg = Conversions::ConvertConfig(config);

				auto pSOEHandler = new SOEHandlerAdapter(handler);
				auto pApplication = new MasterApplicationAdapter(application);
				auto pMasterUserDB = new MasterUserDatabaseAdapter(masterUser);

				auto pMaster = pChannel->AddMaster(stdLoggerId.c_str(), *pSOEHandler, *pApplication, cfg, *pMasterUserDB);
				if (pMaster == nullptr)
				{
					delete pSOEHandler;
					delete pApplication;
					delete pMasterUserDB;
					return nullptr;
				}
				else
				{
					pMaster->DeleteOnDestruct(pSOEHandler);
					pMaster->DeleteOnDestruct(pApplication);
					pMaster->DeleteOnDestruct(pMasterUserDB);
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
					ApplyDatabaseSettings(pOutstation->GetConfigView(), config->databaseTemplate);
					return gcnew OutstationAdapter(pOutstation);
				}
			}

			void ChannelAdapter::ApplyDatabaseSettings(opendnp3::DatabaseConfigView view, DatabaseTemplate^ dbTemplate)
			{
				ApplyIndexClazzAndVariations<BinaryRecord, opendnp3::Binary>(dbTemplate->binaries, view.binaries);
				ApplyIndexClazzAndVariations<DoubleBinaryRecord, opendnp3::DoubleBitBinary>(dbTemplate->doubleBinaries, view.doubleBinaries);
				ApplyIndexClazzAndVariations<BinaryOutputStatusRecord, opendnp3::BinaryOutputStatus>(dbTemplate->binaryOutputStatii, view.binaryOutputStatii);
				
				ApplyIndexClazzDeadbandsAndVariations<CounterRecord, opendnp3::Counter>(dbTemplate->counters, view.counters);
				ApplyIndexClazzDeadbandsAndVariations<FrozenCounterRecord, opendnp3::FrozenCounter>(dbTemplate->frozenCounters, view.frozenCounters);
				ApplyIndexClazzDeadbandsAndVariations<AnalogRecord, opendnp3::Analog>(dbTemplate->analogs, view.analogs);
				ApplyIndexClazzDeadbandsAndVariations<AnalogOutputStatusRecord, opendnp3::AnalogOutputStatus>(dbTemplate->analogOutputStatii, view.analogOutputStatii);

				ApplyStaticVariation<TimeAndIntervalRecord, opendnp3::TimeAndInterval>(dbTemplate->timeAndIntervals, view.timeAndIntervals);				
			}

			void ChannelAdapter::ApplySettings(IReadOnlyList<BinaryRecord^>^ list, openpal::ArrayView < opendnp3::Cell<opendnp3::Binary>, uint16_t>& view)
			{
				ApplyIndexClazzAndVariations<BinaryRecord, opendnp3::Binary>(list, view);
			}

			void ChannelAdapter::Shutdown()
			{
				pChannel->Shutdown();
			}

		}
	}
}
