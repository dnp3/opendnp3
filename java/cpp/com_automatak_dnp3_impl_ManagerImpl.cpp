
#include "com_automatak_dnp3_impl_ManagerImpl.h"

#include <asiodnp3/DNP3Manager.h>

#include "cache/JNI.h"
#include "adapters/LogHandlerAdapter.h"

using namespace asiodnp3;
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
(JNIEnv *, jobject, jlong, jstring, jint, jlong, jlong, jstring, jint)
{
	return 0;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1tcp_1server
(JNIEnv *, jobject, jlong, jstring, jint, jlong, jlong, jstring, jint)
{
	return 0;
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_get_1native_1channel_1serial
(JNIEnv *, jobject, jlong, jstring, jint, jlong, jlong, jstring, jint, jint, jint, jint, jint)
{
	return 0;
}


