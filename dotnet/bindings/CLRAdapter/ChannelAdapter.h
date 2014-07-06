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

ref class ChannelAdapter : IChannel
{
public:

	ChannelAdapter(asiodnp3::IChannel* pChannel_) : pChannel(pChannel_)
	{}	

	virtual LogFilter GetLogFilters() sealed;

	virtual IChannelStatistics^ GetChannelStatistics() sealed;

	virtual void SetLogFilters(LogFilter filters) sealed;

	virtual void AddStateListener(System::Action<ChannelState>^ listener) sealed;

	virtual IMaster^ AddMaster(System::String^ loggerId, ISOEHandler^ publisher, IMasterApplication^ application, MasterStackConfig^ config) sealed;

	virtual IOutstation^ AddOutstation(System::String^ loggerId, ICommandHandler^ cmdHandler, IOutstationApplication^ application, OutstationStackConfig^ config) sealed;

	virtual void Shutdown() sealed;

private:

	asiodnp3::IChannel* pChannel;	

	static void ApplyDatabaseSettings(opendnp3::Database& database, DatabaseTemplate^ dbTemplate);

	template <class Source, class Target>
	static void ApplyClassSettings(Source^ source, Target& target)
	{
		for (int i = 0; i < source->Count; ++i)
		{
			if (target.values.Contains(i))
			{
				target.values[i].current.quality = source[i]->quality;
			}

			if (target.metadata.Contains(i))
			{
				target.metadata[i].clazz = (opendnp3::PointClass) source[i]->pointClass;				
			}			 
		}
	}

	template <class Source, class Target>
	static void ApplyClassAndDeadbandSettings(Source^ source, Target& target)
	{
		for (int i = 0; i < source->Count; ++i)
		{
			if (target.values.Contains(i))
			{
				target.values[i].current.quality = source[i]->quality;
			}

			if (target.metadata.Contains(i))
			{
				target.metadata[i].clazz = (opendnp3::PointClass) source[i]->pointClass;
				target.metadata[i].deadband = source[i]->deadband;
			}
		}
	}
};

}
}

#endif
