
#include "DNP3ManagerWrapper.h"
#include "LogSubscriberAdapter.h"

#include "JNIHelpers.h"

#include <iostream>

namespace apl {
namespace dnp {

DNP3ManagerWrapper::DNP3ManagerWrapper(uint32_t aConcurrency, JavaVM* apJVM) :
	mpJVM(apJVM),	
	mProxy(	aConcurrency,
		[apJVM](){ JNIHelpers::JNIAttachThread(apJVM); }, 
		[apJVM](){ JNIHelpers::JNIDetachThread(apJVM); }
	)	
{
	
}

DNP3ManagerWrapper::~DNP3ManagerWrapper()
{
	mProxy.Shutdown();
	for(auto f: mOnDestruction) f();
}

void DNP3ManagerWrapper::AddLogSubscriber(JNIEnv* apEnv, jobject subscriber)
{
	jobject global = apEnv->NewGlobalRef(subscriber);
	mOnDestruction.push_back([this, global]() { this->ReleaseObject(global); });
	auto pSub = new LogSubscriberAdapter(mpJVM, global);
	mOnDestruction.push_back([pSub](){ delete pSub; });
	mProxy.AddLogSubscriber(pSub);
}

void DNP3ManagerWrapper::ReleaseObject(jobject obj)
{
	JNIEnv* pEnv;
	assert(mpJVM->GetEnv((void **)&pEnv, JNI_VERSION_1_6) == 0);
	assert(pEnv != NULL);
	pEnv->DeleteGlobalRef(obj);
}

void DNP3ManagerWrapper::Shutdown()
{
	mProxy.Shutdown();
}

IChannel* DNP3ManagerWrapper::AddTCPClient(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arAddr, uint16_t aPort)
{
	return mProxy.AddTCPClient(arName, aLevel, aOpenRetry, arAddr, aPort);
}

IChannel* DNP3ManagerWrapper::AddTCPServer(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, const std::string& arEndpoint, uint16_t aPort)
{
	return mProxy.AddTCPServer(arName, aLevel, aOpenRetry, arEndpoint, aPort);
}

IChannel* DNP3ManagerWrapper::AddSerial(const std::string& arName, FilterLevel aLevel, millis_t aOpenRetry, SerialSettings aSettings)
{
	return mProxy.AddSerial(arName, aLevel, aOpenRetry, aSettings);
}

}}

