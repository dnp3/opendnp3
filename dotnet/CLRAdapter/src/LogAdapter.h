#ifndef OPENDNP3CLR_LOG_ADAPTER_H
#define OPENDNP3CLR_LOG_ADAPTER_H

#include <openpal/logging/ILogHandler.h>

#include <memory>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

using namespace System::Collections::ObjectModel;

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
