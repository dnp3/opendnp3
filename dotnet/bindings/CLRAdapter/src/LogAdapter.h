#ifndef __LOG_ADAPTER_H_
#define __LOG_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <openpal/logging/ILogHandler.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class LogAdapter : public openpal::ILogHandler
			{
			public:

				LogAdapter(Automatak::DNP3::Interface::ILogHandler^ proxy);

				// logging error messages, etc
				virtual void Log(const openpal::LogEntry& Entry) override final;

			private:
				gcroot < Automatak::DNP3::Interface::ILogHandler^ > proxy;
			};

			private ref class LogAdapterWrapper
			{
			public:
				LogAdapterWrapper(ILogHandler^ proxy) : adapter(new LogAdapter(proxy))
				{}

				openpal::ILogHandler& GetLogAdapter()
				{
					return *adapter;
				}

				~LogAdapterWrapper()
				{
					delete adapter;
				}

			private:
				LogAdapter* adapter;
			};
		}
	}
}

#endif
