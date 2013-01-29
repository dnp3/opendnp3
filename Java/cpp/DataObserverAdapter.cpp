#include "DataObserverAdapter.h"


using namespace apl;

#include <iostream>

DataObserverAdapter::DataObserverAdapter(JavaVM* apJVM, jobject aProxy) :
mpJVM(apJVM),
mProxy(aProxy)
{
	JNIEnv* pEnv = this->GetEnv();
	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);	

	mStartId = pEnv->GetMethodID(clazz, "start", "()V");
	assert(mStartId != NULL);
	mEndId = pEnv->GetMethodID(clazz, "end", "()V");
	assert(mEndId != NULL);
	
	mUpdateBinaryInput = pEnv->GetMethodID(clazz, "updateBI", "(ZBJJ)V");
	assert(mUpdateBinaryInput != NULL);
	
	mUpdateAnalogInput = pEnv->GetMethodID(clazz, "updateAI", "(DBJJ)V");
	assert(mUpdateAnalogInput != NULL);
	
	mUpdateCounter = pEnv->GetMethodID(clazz, "updateC", "(JBJJ)V");
	assert(mUpdateCounter != NULL);
	
	mUpdateBinaryOutputStatus = pEnv->GetMethodID(clazz, "updateBOS", "(ZBJJ)V");
	assert(mUpdateBinaryOutputStatus != NULL);
	
	mUpdateAnalogOutputStatus = pEnv->GetMethodID(clazz, "updateAOS", "(DBJJ)V");
	assert(mUpdateAnalogOutputStatus != NULL);
}

JNIEnv* DataObserverAdapter::GetEnv()
{
	JNIEnv* pEnv = NULL;
	mpJVM->GetEnv((void **) &pEnv, JNI_VERSION_1_6);
	assert(pEnv != NULL);
	return pEnv;
}

void DataObserverAdapter::_Start()
{
	GetEnv()->CallVoidMethod(mProxy, mStartId);	
}

void DataObserverAdapter::_Update(const Binary& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	pEnv->CallVoidMethod(mProxy, mUpdateBinaryInput, value, quality, timestamp, aIndex);
}

void DataObserverAdapter::_Update(const Analog& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	pEnv->CallVoidMethod(mProxy, mUpdateAnalogInput, value, quality, timestamp, aIndex);
}

void DataObserverAdapter::_Update(const Counter& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jlong value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	pEnv->CallVoidMethod(mProxy, mUpdateCounter, value, quality, timestamp, aIndex);
}

void DataObserverAdapter::_Update(const SetpointStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jdouble value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	pEnv->CallVoidMethod(mProxy, mUpdateAnalogOutputStatus, value, quality, timestamp, aIndex);	
}

void DataObserverAdapter::_Update(const ControlStatus& arMeas, size_t aIndex)
{
	JNIEnv* pEnv = GetEnv();	

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	pEnv->CallVoidMethod(mProxy, mUpdateBinaryOutputStatus, value, quality, timestamp, aIndex);	
}

void DataObserverAdapter::_End()
{
	GetEnv()->CallVoidMethod(mProxy, mEndId);
}
