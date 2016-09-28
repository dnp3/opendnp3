
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include <asiodnp3/DNP3Manager.h>

#include "adapters/CString.h"
#include "adapters/LogHandlerAdapter.h"
#include "adapters/ChannelListenerAdapter.h"

using namespace asiodnp3;
using namespace opendnp3;
using namespace openpal;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
(JNIEnv* env, jobject, jint concurreny, jobject loghandler)
{			
	auto attach = []() { JNI::AttachCurrentThread(); };
	auto detach = []() { JNI::DetachCurrentThread(); };

	auto adapter = std::make_shared<LogHandlerAdapter>(loghandler);	

	return (jlong) new DNP3Manager(concurreny, adapter, attach, detach);	
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_shutdown_1native_1manager
(JNIEnv*, jobject, jlong pointer)
{
	delete (DNP3Manager*) pointer;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1client
(JNIEnv* env, jobject, jlong native, jstring jid, jint jlevels, jlong jminRetry, jlong jmaxRetry, jstring jhost, jstring jadapter, jint jport, jobject jlistener)
{
	const auto manager = (DNP3Manager*) native;

	CString id(env, jid);
	CString host(env, jhost);
	CString adapter(env, jadapter);
	ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));

	auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

	return (jlong) manager->AddTCPClient(id, jlevels, retry, host.str(), adapter.str(), static_cast<uint16_t>(jport), listener);	
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server
(JNIEnv* env, jobject, jlong native, jstring jid, jint jlevels, jlong jminRetry, jlong jmaxRetry, jstring jadapter, jint jport, jobject jlistener)
{
	const auto manager = (DNP3Manager*) native;

	CString id(env, jid);	
	CString adapter(env, jadapter);
	ChannelRetry retry(TimeDuration::Milliseconds(jminRetry), TimeDuration::Milliseconds(jmaxRetry));

	auto listener = jlistener ? std::make_shared<ChannelListenerAdapter>(jlistener) : nullptr;

	return (jlong) manager->AddTCPServer(id, jlevels, retry, adapter.str(), static_cast<uint16_t>(jport), listener);
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial
(JNIEnv *, jobject, jlong, jstring, jint, jlong, jlong, jstring, jint, jint, jint, jint, jint, jobject jlistener)
{
	return 0;
}


