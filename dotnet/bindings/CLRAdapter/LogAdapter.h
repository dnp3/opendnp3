#ifndef __LOG_ADAPTER_H_
#define __LOG_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>
#include <openpal/ILogHandler.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{

private class LogAdapter : public openpal::ILogHandler
{
public:

	LogAdapter(DNP3::Interface::ILogHandler^ proxy);

	// logging error messages, etc
	virtual void Log(const openpal::LogEntry& Entry) override final;

private:
	gcroot < DNP3::Interface::ILogHandler^ > proxy;
};

private ref class LogAdapterWrapper
{
public:
	LogAdapterWrapper(ILogHandler^ proxy) : mpAdapter(new LogAdapter(proxy))
	{}

	openpal::ILogHandler* GetLogAdapter()
	{
		return mpAdapter;
	}

	~LogAdapterWrapper()
	{
		delete mpAdapter;
	}

private:
	LogAdapter* mpAdapter;
};
}
}

#endif
