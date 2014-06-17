#include "ChannelAdapter.h"
#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "OutstationCommandHandlerAdapter.h"
#include "OutstationTimeWriteAdapter.h"
#include "MasterAdapter.h"
#include "OutstationAdapter.h"
#include "DeleteAnything.h"

#include <asiopal/UTCTimeSource.h>

using namespace System::Collections::Generic;

namespace DNP3
{
namespace Adapter
{

ChannelAdapter::ChannelAdapter()
{
	pMultiplexer = new EventMultiplexer<opendnp3::ChannelState, DNP3::Interface::ChannelState>(std::bind(&Conversions::convertChannelState, std::placeholders::_1));
}

ChannelAdapter::~ChannelAdapter()
{
	delete pMultiplexer;
}

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
	return Conversions::convertChannelStats(stats);	
}

void ChannelAdapter::SetLogFilters(LogFilter filters)
{
	openpal::LogFilters flags(filters.Flags);
	pChannel->SetLogFilters(flags);
}

void ChannelAdapter::AddStateListener(System::Action<ChannelState>^ listener)
{
	pMultiplexer->AddListener(listener);
}

void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState aState)
{
	ChannelState state = Conversions::convertChannelState(aState);
	(*listener)->Invoke(state);
}

IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, ISOEHandler^ publisher, MasterStackConfig^ config)
{
	std::string stdLoggerId = Conversions::convertString(loggerId);

	MasterMeasurementHandlerWrapper^ wrapper = gcnew MasterMeasurementHandlerWrapper(publisher);
	opendnp3::MasterStackConfig cfg = Conversions::convertConfig(config);

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
	std::string stdLoggerId = Conversions::convertString(loggerId);

	OutstationCommandHandlerWrapper^ cmdWrapper = gcnew OutstationCommandHandlerWrapper(cmdHandler);
	OutstationTimeWriteWrapper^ timeWrapper = gcnew OutstationTimeWriteWrapper(timeHandler);

	opendnp3::OutstationStackConfig cfg = Conversions::convertConfig(config);

	auto pOutstation = pChannel->AddOutstation(stdLoggerId.c_str(), cmdWrapper->Get(), timeWrapper->Get(), Conversions::convertConfig(config));
	if (pOutstation == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew OutstationAdapter(pOutstation);
	}
}

void ChannelAdapter::Shutdown()
{
	pChannel->BeginShutdown();
}

openpal::IEventHandler<opendnp3::ChannelState>* ChannelAdapter::GetEventHandler()
{
	return this->pMultiplexer;
}

}
}


