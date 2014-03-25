#include "LogAdapter.h"

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


LogAdapter::LogAdapter(ILogHandler^ proxy) : proxy(proxy)
{}

// logging error messages, etc
void LogAdapter::Log( const openpal::LogEntry& arEntry )
{	
	System::String^ loggerName = Conversions::convertString(arEntry.GetName());
	System::String^ location = Conversions::convertString(arEntry.GetLocation());
	System::String^ message = Conversions::convertString(arEntry.GetMessage());

	DNP3::Interface::LogEntry^ le = gcnew DNP3::Interface::LogEntry(arEntry.GetFilters().GetBitfield(), loggerName, location, message, System::DateTime::Now, arEntry.GetErrorCode());

	proxy->Log(le);
}

}
}

