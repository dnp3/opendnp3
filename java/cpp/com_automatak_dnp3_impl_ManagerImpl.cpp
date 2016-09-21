
#include "com_automatak_dnp3_impl_ManagerImpl.h"

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_create_1native_1manager
(JNIEnv *, jobject, jint, jobject)
{
	return 0;
}


JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_ManagerImpl_shutdown_1native_1manager
(JNIEnv *, jobject, jlong)
{

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


