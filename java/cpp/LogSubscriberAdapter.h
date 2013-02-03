#ifndef __LOG_SUBSCRIBER_ADAPTER_H_
#define __LOG_SUBSCRIBER_ADAPTER_H_

#include <jni.h>
#include <opendnp3/LogBase.h>

class LogSubscriberAdapter : public opendnp3::ILogBase
{
	public:
	LogSubscriberAdapter(JavaVM* apJVM, jobject aProxy);

	void Log(const opendnp3::LogEntry& arEntry);

	private:
	JavaVM* mpJVM;
	jobject mProxy;
};

#endif
