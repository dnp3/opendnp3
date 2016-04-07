#ifndef __LOG_ADAPTER_H_
#define __LOG_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <openpal/logging/ILogHandler.h>

#include <vcclr.h>
#include <memory>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class LogAdapter final : public openpal::ILogHandler
			{
			public:

				LogAdapter(Automatak::DNP3::Interface::ILogHandler^ proxy);

				static std::shared_ptr<openpal::ILogHandler> Create(Automatak::DNP3::Interface::ILogHandler^ proxy);

				// logging error messages, etc
				virtual void Log(const openpal::LogEntry& Entry) override;

			private:
				gcroot < Automatak::DNP3::Interface::ILogHandler^ > proxy;
			};
			
		}
	}
}

#endif
