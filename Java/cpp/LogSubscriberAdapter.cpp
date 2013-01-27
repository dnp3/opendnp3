#include "LogSubscriberAdapter.h"

#include <iostream>
#include <chrono>

#include "JNIHelpers.h"

LogSubscriberAdapter::LogSubscriberAdapter(JavaVM* apJVM, jobject aProxy) :
mpJVM(apJVM),
mProxy(aProxy)
{

}

void LogSubscriberAdapter::Log(const apl::LogEntry& arEntry)
{	
	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);	

	jclass leClz = pEnv->FindClass("com/automatak/dnp3/impl/LogEntryImpl");
	assert(leClz != NULL);				   
	jmethodID mid = pEnv->GetMethodID(leClz, "<init>","(ILjava/lang/String;Ljava/lang/String;JI)V");
	assert(mid != NULL);
	
	jint level = arEntry.GetFilterLevel();
	jstring name = pEnv->NewStringUTF(arEntry.GetDeviceName().c_str());
	jstring msg = pEnv->NewStringUTF(arEntry.GetMessage().c_str());
	jlong time = std::chrono::duration_cast<std::chrono::milliseconds>(arEntry.GetTimeStamp().time_since_epoch()).count();
	jint error = arEntry.GetErrorCode();

	jobject le = pEnv->NewObject(leClz, mid, level, name, msg, time, error);

	assert(le != NULL);

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);				
	jmethodID mid2 = pEnv->GetMethodID(clazz, "onLogEntry", "(Lcom/automatak/dnp3/LogEntry;)V");
	assert(mid2 != NULL);	

	pEnv->CallVoidMethod(mProxy, mid2, le);
}
