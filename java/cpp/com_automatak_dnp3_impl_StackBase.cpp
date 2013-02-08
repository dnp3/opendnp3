#include "com_automatak_dnp3_impl_StackBase.h"

#include <opendnp3/IStack.h>
#include <opendnp3/VtoRouterSettings.h>

#include "JNIHelpers.hpp"

using namespace opendnp3;

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_StackBase_get_1tcpclient_1vto_1endpoint
  (JNIEnv* apEnv, jobject, jlong nativeStack, jstring loggerId, jint logLevel, jstring host, jint port, jbyte channel, jlong minRetry, jlong maxRetry, jboolean startLocal, jboolean disableExt)
{
	auto pStack = (IStack*) nativeStack;
	VtoRouterSettings settings(channel, minRetry, maxRetry, startLocal, disableExt);
	std::string logger = JNIHelpers::GetString(loggerId, apEnv);
	FilterLevel level = LogTypes::ConvertIntToFilterLevel(logLevel);
	std::string address = JNIHelpers::GetString(host, apEnv);
	return (jlong) pStack->StartVtoRouterTCPClient(logger, level, address, port, settings); 
}

JNIEXPORT jlong JNICALL Java_com_automatak_dnp3_impl_StackBase_get_1tcpserver_1vto_1endpoint
  (JNIEnv* apEnv, jobject, jlong nativeStack, jstring loggerId, jint logLevel, jstring adapter, jint port, jbyte channel, jlong minRetry, jlong maxRetry, jboolean startLocal, jboolean disableExt)
{
	auto pStack = (IStack*) nativeStack;
	VtoRouterSettings settings(channel, minRetry, maxRetry, startLocal, disableExt);
	std::string logger = JNIHelpers::GetString(loggerId, apEnv);
	FilterLevel level = LogTypes::ConvertIntToFilterLevel(logLevel);
	std::string address = JNIHelpers::GetString(adapter, apEnv);
	return (jlong) pStack->StartVtoRouterTCPServer(logger, level, address, port, settings);
}


