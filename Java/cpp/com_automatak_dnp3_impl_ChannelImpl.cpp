#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <DNP3/IChannel.h>
#include <DNP3/IMaster.h>
#include <DNP3/IOutstation.h>

#include "JNIHelpers.h"
#include "DataObserverAdapter.h"
#include "CommandHandlerAdapter.h"

using namespace apl;
using namespace apl::dnp;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
  (JNIEnv *, jobject, jlong ptr)
{
	auto pChannel = (IChannel*) ptr;
	pChannel->Shutdown();
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject jlogLevel, jobject publisher)
{
	auto pChannel = (IChannel*) ptr;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);
	jobject global = pEnv->NewGlobalRef(publisher);
	auto pPublisher = new DataObserverAdapter(pJVM, global);
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	auto pMaster = pChannel->AddMaster(loggerId, LEV_INFO, pPublisher, MasterStackConfig());
	pMaster->AddDestructorHook([pJVM, global]() { JNIHelpers::DeleteGlobalReference(pJVM, global); });
	pMaster->AddDestructorHook([pPublisher](){ delete pPublisher; });
	return (jlong) pMaster;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject jloglevel, jobject commandAdapter)
{
	auto pChannel = (IChannel*) ptr;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	SlaveStackConfig config;
	DeviceTemplate templ(3,3,3,3,3);
	config.device = templ;
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(pEnv);	
	jobject global = pEnv->NewGlobalRef(commandAdapter);
	auto pCmdHandler = new CommandHandlerAdapter(pJVM, global);	
	auto pOutstation = pChannel->AddOutstation(loggerId, LEV_INFO, pCmdHandler, config);
	pOutstation->AddDestructorHook([pJVM, global]() { JNIHelpers::DeleteGlobalReference(pJVM, global); });
	pOutstation->AddDestructorHook([pCmdHandler](){ delete pCmdHandler; });
	return (jlong) pOutstation;
}

