#ifndef __CHANNEL_ADAPTER_H_
#define __CHANNEL_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IChannel.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
//void CallbackListener(gcroot < System::Action<ChannelState> ^ >* listener, opendnp3::ChannelState);

private ref class ChannelAdapter : IChannel
{
public:

	ChannelAdapter();	

	void SetChannel(asiodnp3::IChannel* pChannel_);

	virtual LogFilter GetLogFilters();

	virtual IChannelStatistics^ GetChannelStatistics();

	virtual void SetLogFilters(LogFilter filters);

	virtual void AddStateListener(System::Action<ChannelState>^ listener);

	virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, MasterStackConfig^ config);

	virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, ITimeWriteHandler^ timeHandler, OutstationStackConfig^ config);

	virtual void Shutdown();	

private:

	asiodnp3::IChannel* pChannel;	
};

}
}

#endif
