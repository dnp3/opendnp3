#include "ChannelAdapter.h"
#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "OutstationApplicationAdapter.h"
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

IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ handler, MasterStackConfig^ config)
{
	std::string stdLoggerId = Conversions::ConvertString(loggerId);

	auto pSOEHandler = new SOEHandlerAdapter(handler);
	opendnp3::MasterStackConfig cfg = Conversions::ConvertConfig(config);

	// TODO expose time source via wrapper
	auto pMaster = pChannel->AddMaster(stdLoggerId.c_str(), *pSOEHandler, asiopal::UTCTimeSource::Instance(), opendnp3::DefaultMasterApplication::Instance(), cfg);
	if (pMaster == nullptr)
	{
		delete pSOEHandler;
		return nullptr;
	}
	else
	{
		pMaster->DeleteOnDestruct(pSOEHandler);
		return gcnew MasterAdapter(pMaster);
	}
}

IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config)
{
	std::string stdLoggerId = Conversions::ConvertString(loggerId);

	auto pCommand = new OutstationCommandHandlerAdapter(cmdHandler);
	auto pApplication = new OutstationApplicationAdapter(application);

	opendnp3::OutstationStackConfig cfg = Conversions::ConvertConfig(config);

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


