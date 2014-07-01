#ifndef __CHANNEL_ADAPTER_H_
#define __CHANNEL_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <asiodnp3/IChannel.h>
#include <vcclr.h>

#include <opendnp3/outstation/Database.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private ref class ChannelAdapter : IChannel
{
public:

	ChannelAdapter(asiodnp3::IChannel* pChannel_) : pChannel(pChannel_)
	{}	

	virtual LogFilter GetLogFilters();

	virtual IChannelStatistics^ GetChannelStatistics();

	virtual void SetLogFilters(LogFilter filters);

	virtual void AddStateListener(System::Action<ChannelState>^ listener);

	virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, MasterStackConfig^ config);

	virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config);

	virtual void Shutdown();	

private:

	asiodnp3::IChannel* pChannel;	

	static void ApplyDatabaseSettings(opendnp3::Database& database, DatabaseTemplate^ dbTemplate);

	template <class Source, class Target>
	static void ApplyClassSettings(Source^ source, Target& target)
	{
		for (int i = 0; i < source->Count; ++i)
		{
			if (target.Contains(i))
			{
				target[i].clazz = (opendnp3::PointClass) source[i]->pointClass;
			}			 
		}
	}

	template <class Source, class Target>
	static void ApplyClassAndDeadbandSettings(Source^ source, Target& target)
	{
		for (int i = 0; i < source->Count; ++i)
		{
			if (target.Contains(i))
			{
				target[i].clazz = (opendnp3::PointClass) source[i]->pointClass;
				target[i].deadband = source[i]->deadband;
			}
		}
	}
};

}
}

#endif
