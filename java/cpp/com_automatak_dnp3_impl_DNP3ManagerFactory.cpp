
#include "com_automatak_dnp3_impl_DNP3ManagerFactory.h"

#include "jni\JCache.h"

JNIEXPORT jboolean JNICALL Java_com_automatak_dnp3_impl_DNP3ManagerFactory_init_1jni_1cache
(JNIEnv* env, jclass)
{
	return jcache::init(env);
}

