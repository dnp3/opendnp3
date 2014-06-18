#include "LogAdapter.h"

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


LogAdapter::LogAdapter(ILogHandler^ proxy) : proxy(proxy)
{}

// logging error messages, etc
void LogAdapter::Log( const openpal::LogEntry& entry )
{
	System::String^ loggerName = Conversions::convertString(entry.GetId());
	System::String^ location = Conversions::convertString(entry.GetLocation());
	System::String^ message = Conversions::convertString(entry.GetMessage());

	DNP3::Interface::LogEntry^ le = gcnew DNP3::Interface::LogEntry(entry.GetFilters().GetBitfield(), loggerName, location, message, System::DateTime::Now, entry.GetErrorCode());

	proxy->Log(le);
}

}
}

