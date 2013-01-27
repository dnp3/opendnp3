#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <DNP3/IChannel.h>

#include "JNIHelpers.h"

#include <APL/SimpleDataObserver.h>
#include <DNP3/SimpleCommandHandler.h>

using namespace apl;
using namespace apl::dnp;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
  (JNIEnv *, jobject, jlong ptr)
{
	auto pChannel = (IChannel*) ptr;
	pChannel->Shutdown();
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject jlogLevel)
{
	auto pChannel = (IChannel*) ptr;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	auto pMaster = pChannel->AddMaster(loggerId, LEV_INFO, PrintingDataObserver::Inst(), MasterStackConfig());
	return (jlong) pMaster;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1slave
  (JNIEnv* pEnv, jobject, jlong ptr, jstring jloggerId, jobject)
{
	auto pChannel = (IChannel*) ptr;
	std::string loggerId = JNIHelpers::GetString(jloggerId, pEnv);
	auto pOutstation = pChannel->AddOutstation(loggerId, LEV_INFO, SuccessCommandHandler::Inst(), SlaveStackConfig());
	return (jlong) pOutstation;
}

