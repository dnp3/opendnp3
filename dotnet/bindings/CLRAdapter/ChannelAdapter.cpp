#include "ChannelAdapter.h"
#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "OutstationTimeWriteAdapter.h"
#include "MasterAdapter.h"
#include "OutstationAdapter.h"
#include "DeleteAnything.h"
#include "EventConverter.h"

#include <opendnp3/outstation/Database.h>

#include <asiopal/UTCTimeSource.h>
#include <functional>

using namespace System::Collections::Generic;

namespace DNP3
{
namespace Adapter
{

void ChannelAdapter::SetChannel(asiodnp3::IChannel* pChannel_)
{
	pChannel = pChannel_;	
}

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
	auto pConverter = new EventConverter<opendnp3::ChannelState, DNP3::Interface::ChannelState>(convert, listener);
	std::function<void (opendnp3::ChannelState)> trigger = pConverter->GetTrigger();
	pChannel->AddStateListener(trigger);
	pChannel->DeleteOnDestruct(pConverter);
}

void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState aState)
{
	ChannelState state = Conversions::ConvertChannelState(aState);
	(*listener)->Invoke(state);
}

IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ publisher, MasterStackConfig^ config)
{
	std::string stdLoggerId = Conversions::ConvertString(loggerId);

	MasterMeasurementHandlerWrapper^ wrapper = gcnew MasterMeasurementHandlerWrapper(publisher);
	opendnp3::MasterStackConfig cfg = Conversions::ConvertConfig(config);

	auto pMaster = pChannel->AddMaster(stdLoggerId.c_str(), wrapper->Get(), asiopal::UTCTimeSource::Inst(), cfg); // TODO expose time source
	if (pMaster == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew MasterAdapter(pMaster);
	}
}

IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, ITimeWriteHandler^ timeHandler, OutstationStackConfig^ config)
{
	std::string stdLoggerId = Conversions::ConvertString(loggerId);

	OutstationCommandHandlerWrapper^ cmdWrapper = gcnew OutstationCommandHandlerWrapper(cmdHandler);
	OutstationTimeWriteWrapper^ timeWrapper = gcnew OutstationTimeWriteWrapper(timeHandler);

	opendnp3::OutstationStackConfig cfg = Conversions::ConvertConfig(config);

	auto pOutstation = pChannel->AddOutstation(stdLoggerId.c_str(), cmdWrapper->Get(), timeWrapper->Get(), Conversions::ConvertConfig(config));
	if (pOutstation == nullptr)
	{
		return nullptr;
	}
	else
	{		
		ApplyDatabaseSettings(pOutstation->GetDatabase(), config->databaseTemplate);
		return gcnew OutstationAdapter(pOutstation);
	}
}

void ChannelAdapter::ApplyDatabaseSettings(opendnp3::Database& database, DatabaseTemplate^ dbTemplate)
{
	ApplyClassSettings(dbTemplate->binaries, database.staticData.binaries.metadata);
	ApplyClassSettings(dbTemplate->binaryOutputStatii, database.staticData.binaryOutputStatii.metadata);
	ApplyClassSettings(dbTemplate->doubleBinaries, database.staticData.doubleBinaries.metadata);

	ApplyClassAndDeadbandSettings(dbTemplate->analogs, database.staticData.analogs.metadata);
	ApplyClassAndDeadbandSettings(dbTemplate->counters, database.staticData.counters.metadata);
	ApplyClassAndDeadbandSettings(dbTemplate->frozenCounters, database.staticData.frozenCounters.metadata);
	ApplyClassAndDeadbandSettings(dbTemplate->analogOutputStatii, database.staticData.analogOutputStatii.metadata);

}

void ChannelAdapter::Shutdown()
{
	pChannel->Shutdown();
}

}
}


