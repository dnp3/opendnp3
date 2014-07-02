
#include "Stdafx.h"
#include "LogAdapter.h"

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{


LogAdapter::LogAdapter(DNP3::Interface::ILogHandler^ proxy) : proxy(proxy)
{}

// logging error messages, etc
void LogAdapter::Log( const openpal::LogEntry& entry )
{
	System::String^ alias = Conversions::ConvertString(entry.GetAlias());
	System::String^ location = Conversions::ConvertString(entry.GetLocation());
	System::String^ message = Conversions::ConvertString(entry.GetMessage());

	DNP3::Interface::LogEntry^ le = gcnew DNP3::Interface::LogEntry(entry.GetFilters().GetBitfield(), alias, location, message, entry.GetErrorCode());

	proxy->Log(le);
}

}
}

