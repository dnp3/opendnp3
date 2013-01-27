#include "com_automatak_dnp3_impl_ManagerImpl.h"


#include "JNIHelpers.h"
#include "LogSubscriberAdapter.h"
#include "DNP3ManagerWrapper.h"

using namespace apl;
using namespace apl::dnp;

#include <iostream>

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
  (JNIEnv* apEnv, jobject, jint concurrency)
{
	JavaVM* pJVM;
	apEnv->GetJavaVM(&pJVM);
	assert(pJVM != NULL);
	auto pManager = new DNP3ManagerWrapper(concurrency, pJVM);
	return (jlong) pManager;
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_destroy_1native_1manager
  (JNIEnv *, jobject, jlong ptrManager)
{
	delete (DNP3ManagerWrapper*) ptrManager;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client
  (JNIEnv * pEnv, jobject, jlong ptrManager, jstring jloggerId, jobject logLevel, jlong timeoutMs, jstring jhost, jint port)
{
	auto pMgr = (DNP3ManagerWrapper*) ptrManager;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	std::string host = JNIHelpers::GetString(jhost, pEnv);
	return (jlong) pMgr->AddTCPClient(loggerId, LEV_INFO, timeoutMs, host, port);
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_native_1add_1log_1subscriber
  (JNIEnv* pEnv, jobject, jlong ptrManager, jobject jsubscriber)
{
	auto pMgr = (DNP3ManagerWrapper*) ptrManager;	
	pMgr->AddLogSubscriber(pEnv, jsubscriber);
}
