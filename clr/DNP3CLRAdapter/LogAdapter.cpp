#include "LogAdapter.h"

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


LogAdapter::LogAdapter(ILogHandler ^ proxy) : proxy(proxy)
{}

// logging error messages, etc
void LogAdapter::Log( const openpal::LogEntry& arEntry )
{
	DNP3::Interface::LogLevel level = Conversions::convertLogLevel(arEntry.GetLogLevel());
	System::String ^ loggerName = Conversions::convertString(arEntry.GetDeviceName());
	System::String ^ location = Conversions::convertString(arEntry.GetLocation());
	System::String ^ message = Conversions::convertString(arEntry.GetMessage());
	
	DNP3::Interface::LogEntry ^ le = gcnew DNP3::Interface::LogEntry(level, loggerName, location, message, System::DateTime::Now, arEntry.GetErrorCode());

	proxy->Log(le);
}

}
}

