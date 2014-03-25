#ifndef __CHANNEL_ADAPTER_H_
#define __CHANNEL_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IChannel.h>
#include <vcclr.h>

#include <openpal/IEventHandler.h>

#include "EventMultiplexer.h"

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState);

private ref class ChannelAdapter : IChannel
{
public:

	ChannelAdapter();
	~ChannelAdapter();
	
	void SetChannel(opendnp3::IChannel* pChannel_);

	virtual LogFilter GetLogFilters();

	virtual void SetLogFilters(LogFilter filters);

	virtual void AddStateListener(System::Action<ChannelState>^ listener);

	virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, MasterStackConfig^ config);

	virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, ITimeWriteHandler^ timeHandler, SlaveStackConfig^ config);

	virtual void Shutdown();

	openpal::IEventHandler<opendnp3::ChannelState>* GetEventHandler();

private:

	opendnp3::IChannel* pChannel;
	EventMultiplexer<opendnp3::ChannelState, ChannelState>* pMultiplexer;
};

}
}

#endif
