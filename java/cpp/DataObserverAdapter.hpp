/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#ifndef __DATA_OBSERVER_ADAPTER_H_
#define __DATA_OBSERVER_ADAPTER_H_

#include <jni.h>
#include <opendnp3/outstation/IDataObserver.h>
#include <opendnp3/master/ISOEHandler.h>

class DataObserverAdapter : public opendnp3::ISOEHandler
{

public:
	DataObserverAdapter(JavaVM* apJVM, jobject aProxy);
	
	virtual void Start() final override {}
	virtual void End() final override {}

	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) {}
	virtual void LoadStatic(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) {}

	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Binary, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::DoubleBitBinary, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Analog, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::Counter, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::FrozenCounter, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::BinaryOutputStatus, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::AnalogOutputStatus, uint16_t>>& meas) {}
	virtual void LoadEvent(const opendnp3::IterableBuffer<opendnp3::IndexedValue<opendnp3::OctetString, uint16_t>>& meas) {}
	

private:

	void _Start();
	void _Update(const opendnp3::Binary& arMeas, size_t aIndex);
	void _Update(const opendnp3::Analog& arMeas, size_t aIndex);
	void _Update(const opendnp3::Counter& arMeas, size_t aIndex);
	void _Update(const opendnp3::AnalogOutputStatus& arMeas, size_t aIndex);
	void _Update(const opendnp3::BinaryOutputStatus& arMeas, size_t aIndex);
	void _End();
	
	JNIEnv* GetEnv();

	JavaVM* mpJVM;
	jobject mProxy;

	jmethodID mStartId;
	jmethodID mEndId;

	// BinaryInput
	jmethodID mUpdateBinaryInput;

	// AnalogInput
	jmethodID mUpdateAnalogInput;

	// Counter
	jmethodID mUpdateCounter;

	// BinaryOutputStatus
	jmethodID mUpdateBinaryOutputStatus;

	// AnalogOutputStatus
	jmethodID mUpdateAnalogOutputStatus;
};

#endif
