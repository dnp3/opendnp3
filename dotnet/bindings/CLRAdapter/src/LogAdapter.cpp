
#include "LogAdapter.h"

#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{


			LogAdapter::LogAdapter(Automatak::DNP3::Interface::ILogHandler^ proxy) : proxy(proxy)
			{}

			std::shared_ptr<openpal::ILogHandler> LogAdapter::Create(Automatak::DNP3::Interface::ILogHandler^ proxy)
			{
				return std::shared_ptr<openpal::ILogHandler>(new LogAdapter(proxy));
			}

			// logging error messages, etc
			void LogAdapter::Log(const openpal::LogEntry& entry)
			{
				System::String^ alias = Conversions::ConvertString(entry.GetAlias());
				System::String^ location = Conversions::ConvertString(entry.GetLocation());
				System::String^ message = Conversions::ConvertString(entry.GetMessage());

				Automatak::DNP3::Interface::LogEntry^ le = gcnew Automatak::DNP3::Interface::LogEntry(entry.GetFilters().GetBitfield(), alias, location, message, entry.GetErrorCode());

				proxy->Log(le);
			}			

		}
	}
}