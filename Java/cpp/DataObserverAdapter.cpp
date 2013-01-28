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

	jclass mclazz = pEnv->FindClass("com/automatak/dnp3/BinaryInput");
	assert(mclazz != NULL);				   
	jmethodID mid = pEnv->GetMethodID(mclazz, "<init>","(ZBJ)V");
	assert(mid != NULL);

	jboolean value = arMeas.GetValue();
	jbyte quality = arMeas.GetQuality();
	jlong timestamp = arMeas.GetTime();

	jobject meas = pEnv->NewObject(mclazz, mid, value, quality, timestamp);

	jclass clazz = pEnv->GetObjectClass(mProxy);
	assert(clazz != NULL);				
	jmethodID mid2 = pEnv->GetMethodID(clazz, "update", "(Lcom/automatak/dnp3/BinaryInput;J)V");
	assert(mid2 != NULL);	

	pEnv->CallVoidMethod(mProxy, mid2, meas, aIndex);
}

void DataObserverAdapter::_Update(const Analog& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const Counter& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const SetpointStatus& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_Update(const ControlStatus& arMeas, size_t aIndex)
{

}

void DataObserverAdapter::_End()
{
	GetEnv()->CallVoidMethod(mProxy, mEndId);
}
