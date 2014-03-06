#ifndef __CHANNEL_ADAPTER_H_
#define __CHANNEL_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IChannel.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
void CallbackListener(gcroot < System::Action<ChannelState> ^ > * listener, opendnp3::ChannelState);

private ref class ChannelAdapter : IChannel
{
public:

	ChannelAdapter(opendnp3::IChannel* apChannel);

	virtual void AddStateListener(System::Action<ChannelState> ^ listener);

	virtual IMaster ^ AddMaster(System::String ^ loggerId, DNP3::Interface::LogLevel level, ISOEHandler ^ publisher, MasterStackConfig ^ config);

	virtual IOutstation ^ AddOutstation(System::String ^ loggerId, DNP3::Interface::LogLevel level, ICommandHandler ^ cmdHandler, ITimeWriteHandler ^ timeHandler, SlaveStackConfig ^ config);

	virtual void Shutdown();

private:

	opendnp3::IChannel* mpChannel;
};

}
}

#endif
