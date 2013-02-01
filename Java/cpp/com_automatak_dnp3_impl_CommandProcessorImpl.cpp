#include "com_automatak_dnp3_impl_CommandProcessorImpl.h"

#include <DNP3/ICommandProcessor.h>
#include "JNIHelpers.h"

using namespace apl::dnp;

void DoCommandCallback(JavaVM* apJVM, jobject gref, CommandResponse rsp)
{
	JNIEnv* pEnv = JNIHelpers::GetEnvFromJVM(apJVM);
	jclass clazz = pEnv->GetObjectClass(gref);
	jmethodID mid = pEnv->GetMethodID(clazz, "onStatusCallback", "(I)V");
	pEnv->CallVoidMethod(gref, mid, rsp.mResult);
	pEnv->DeleteGlobalRef(gref);
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_selectAndOperateCROB
  (JNIEnv * apEnv, jobject, jlong ptr, jint code, jshort count, jlong onTime, jlong offTime, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	ControlRelayOutputBlock crob(IntToControlCode(code), count, onTime, offTime);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->SelectAndOperate(crob, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_directOperateCROB
  (JNIEnv * apEnv, jobject, jlong ptr, jint code, jshort count, jlong onTime, jlong offTime, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	ControlRelayOutputBlock crob(IntToControlCode(code), count, onTime, offTime);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->DirectOperate(crob, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_selectAndOperateAnalogInt32
  (JNIEnv * apEnv, jobject, jlong ptr, jint value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputInt32 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->SelectAndOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_directOperateAnalogInt32
  (JNIEnv * apEnv, jobject, jlong ptr, jint value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputInt32 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->DirectOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_selectAndOperateAnalogInt16
  (JNIEnv * apEnv, jobject, jlong ptr, jshort value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputInt16 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->SelectAndOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_directOperateAnalogInt16
  (JNIEnv * apEnv, jobject, jlong ptr, jshort value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputInt16 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->DirectOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_selectAndOperateAnalogFloat32
  (JNIEnv * apEnv, jobject, jlong ptr, jfloat value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputFloat32 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->SelectAndOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_directOperateAnalogFloat32
  (JNIEnv * apEnv, jobject, jlong ptr, jfloat value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputFloat32 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->DirectOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_selectAndOperateAnalogDouble64
  (JNIEnv * apEnv, jobject, jlong ptr, jdouble value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputDouble64 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->SelectAndOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}

JNIEXPORT void JNICALL Java_com_automatak_dnp3_impl_CommandProcessorImpl_directOperateAnalogDouble64
  (JNIEnv * apEnv, jobject, jlong ptr, jdouble value, jint status, jlong index, jobject callback)
{
	auto pCmdProcessor = (ICommandProcessor*) ptr;
	AnalogOutputDouble64 ao(value);
	jobject gref = apEnv->NewGlobalRef(callback);
	JavaVM* pJVM = JNIHelpers::GetJVMFromEnv(apEnv);
	pCmdProcessor->DirectOperate(ao, index, [=](CommandResponse rsp){ DoCommandCallback(pJVM, gref, rsp); });
}