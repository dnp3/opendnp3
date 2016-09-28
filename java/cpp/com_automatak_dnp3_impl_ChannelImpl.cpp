
#include "com_automatak_dnp3_impl_ChannelImpl.h"

#include <asiodnp3/IChannel.h>

#include "adapters/ConfigReader.h"
#include "adapters/SOEHandlerAdapter.h"
#include "adapters/MasterApplicationAdapter.h"

#include "adapters/CString.h"

#include <memory>

using namespace asiodnp3;
using namespace opendnp3;

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_shutdown_1native
(JNIEnv* env, jobject, jlong native)
{
	const auto channel = (IChannel*) native;
	channel->Shutdown();
}


JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1master
(JNIEnv* env, jobject, jlong native, jstring jid, jobject handler, jobject application, jobject jconfig)
{
	const auto channel = (IChannel*)native;

	auto config = ConfigReader::ConvertMasterStackConfig(env, jconfig);
	auto soeAdapter = std::make_shared<SOEHandlerAdapter>(handler);
	auto appAdapter = std::make_shared<MasterApplicationAdapter>(application);

	CString id(env, jid);

	return (jlong) channel->AddMaster(id, soeAdapter, appAdapter, config);	 
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ChannelImpl_get_1native_1outstation
(JNIEnv* env, jobject, jlong native, jstring id, jobject commandAdapter, jobject application, jobject jconfig)
{
	return 0;
}

