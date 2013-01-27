#ifndef __LOG_SUBSCRIBER_ADAPTER_H_
#define __LOG_SUBSCRIBER_ADAPTER_H_

#include <jni.h>
#include <APL/LogBase.h>

class LogSubscriberAdapter : public apl::ILogBase
{
	public:
	LogSubscriberAdapter(JavaVM* apJVM, jobject aProxy);

	void Log(const apl::LogEntry& arEntry);

	private:
	JavaVM* mpJVM;
	jobject mProxy;
};

#endif
