#ifndef __CHANNEL_ADAPTER_H_
#define __CHANNEL_ADAPTER_H_

using namespace System::Collections::Generic;
using namespace System::Collections::ObjectModel;

#include <asiodnp3/IChannel.h>
#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
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

				static void ApplyDatabaseSettings(opendnp3::DatabaseConfigView view, DatabaseTemplate^ dbTemplate);

				static void ApplySettings(IReadOnlyList<BinaryRecord^>^ list, openpal::ArrayView < opendnp3::Cell<opendnp3::Binary>, uint16_t>& view);

				template <class Managed, class Native>
				static void ApplyStaticVariation(IReadOnlyList<Managed^>^ list, openpal::ArrayView < opendnp3::Cell<Native>, uint16_t>& view)
				{
					for (int i = 0; i < view.Size(); ++i)
					{
						view[i].vIndex = list[i]->index;
						view[i].variation = (typename Native::StaticVariation) list[i]->staticVariation;						
					}
				}

				template <class Managed, class Native>
				static void ApplyIndexClazzAndVariations(IReadOnlyList<Managed^>^ list, openpal::ArrayView < opendnp3::Cell<Native>, uint16_t>& view)
				{
					for (int i = 0; i < view.Size(); ++i)
					{						
						view[i].vIndex = list[i]->index;						
						view[i].variation = (typename Native::StaticVariation) list[i]->staticVariation;
						view[i].metadata.variation = (typename Native::EventVariation) list[i]->eventVariation;
						view[i].metadata.clazz = (opendnp3::PointClass) list[i]->clazz;
					}
				}

				template <class Managed, class Native>
				static void ApplyIndexClazzDeadbandsAndVariations(IReadOnlyList<Managed^>^ list, openpal::ArrayView < opendnp3::Cell<Native>, uint16_t>& view)
				{
					for (int i = 0; i < view.Size(); ++i)
					{
						view[i].vIndex = list[i]->index;
						view[i].variation = (typename Native::StaticVariation) list[i]->staticVariation;
						view[i].metadata.variation = (typename Native::EventVariation) list[i]->eventVariation;
						view[i].metadata.deadband = list[i]->deadband;
						view[i].metadata.clazz = (opendnp3::PointClass) list[i]->clazz;
					}
				}
				
			};

		}
	}
}

#endif
