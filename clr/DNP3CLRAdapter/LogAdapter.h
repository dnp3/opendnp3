#ifndef __LOG_ADAPTER_H_
#define __LOG_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <openpal/LogBase.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private class LogAdapter : public openpal::ILogBase
{
public:

	LogAdapter(ILogHandler ^ proxy);

	// logging error messages, etc
	void Log( const openpal::LogEntry& arEntry );

private:
	gcroot < ILogHandler ^ > proxy;
};

private ref class LogAdapterWrapper
{
public:
	LogAdapterWrapper(ILogHandler ^ proxy) : mpAdapter(new LogAdapter(proxy))
	{}

	openpal::ILogBase* GetLogAdapter() {
		return mpAdapter;
	}

	~LogAdapterWrapper() {
		delete mpAdapter;
	}

private:
	LogAdapter* mpAdapter;
};
}
}

#endif
