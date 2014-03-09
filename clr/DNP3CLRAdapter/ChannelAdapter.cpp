#include "ChannelAdapter.h"
#include "Conversions.h"
#include "SOEHandlerAdapter.h"
#include "SlaveCommandHandlerAdapter.h"
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


ChannelAdapter::ChannelAdapter(opendnp3::IChannel* apChannel) :
	mpChannel(apChannel)
{

}

void ChannelAdapter::AddStateListener(System::Action<ChannelState>^ listener)
{
	auto pWrapper = new gcroot < System::Action<ChannelState> ^ > (listener);
	mpChannel->AddDestructorHook(std::bind(&DeleteAnything < gcroot < System::Action<ChannelState> ^ >> , pWrapper));
	mpChannel->AddStateListener(std::bind(&CallbackListener, pWrapper, std::placeholders::_1));
}

void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState aState)
{
	ChannelState state = Conversions::convertChannelState(aState);
	(*listener)->Invoke(state);
}

IMaster^ ChannelAdapter::AddMaster(System::String^ loggerId, DNP3::Interface::LogLevel level, ISOEHandler^ publisher, MasterStackConfig^ config)
{
	std::string stdLoggerId = Conversions::convertString(loggerId);
	openpal::LogLevel stdLevel = Conversions::convertLogLevel(level);

	MasterMeasurementHandlerWrapper^ wrapper = gcnew MasterMeasurementHandlerWrapper(publisher);
	opendnp3::MasterStackConfig cfg = Conversions::convertConfig(config);

	auto pMaster = mpChannel->AddMaster(stdLoggerId, stdLevel, wrapper->Get(), asiopal::UTCTimeSource::Inst(), cfg); // TODO expose time source
	if (pMaster == nullptr)
	{
		return nullptr;
	}
	else
	{
		return gcnew MasterAdapter(pMaster);
	}
}

IOutstation^ ChannelAdapter::AddOutstation(System::String^ loggerId, DNP3::Interface::LogLevel level, ICommandHandler^ cmdHandler, ITimeWriteHandler^ timeHandler, SlaveStackConfig^ config)
{
	std::string stdLoggerId = Conversions::convertString(loggerId);
	openpal::LogLevel stdLevel = Conversions::convertLogLevel(level);

	SlaveCommandHandlerWrapper^ cmdWrapper = gcnew SlaveCommandHandlerWrapper(cmdHandler);
	OutstationTimeWriteWrapper^ timeWrapper = gcnew OutstationTimeWriteWrapper(timeHandler);

	opendnp3::SlaveStackConfig cfg = Conversions::convertConfig(config);

	auto pOutstation = mpChannel->AddOutstation(stdLoggerId, stdLevel, cmdWrapper->Get(), timeWrapper->Get(), Conversions::convertConfig(config));
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
	mpChannel->Shutdown();
}

}
}


