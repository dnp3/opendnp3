/**
 * Copyright 2013 Automatak, LLC
 *
 * Licensed to Automatak, LLC (www.automatak.com) under one or more
 * contributor license agreements. See the NOTICE file distributed with this
 * work for additional information regarding copyright ownership. Automatak, LLC
 * licenses this file to you under the GNU Affero General Public License
 * Version 3.0 (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 * http://www.gnu.org/licenses/agpl.html
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
#include <opendnp3/IDataObserver.h>

class DataObserverAdapter : public opendnp3::IDataObserver
{
	public:
	DataObserverAdapter(JavaVM* apJVM, jobject aProxy);

	protected:

	void _Start();
	void _Update(const opendnp3::Binary& arMeas, size_t aIndex);
	void _Update(const opendnp3::Analog& arMeas, size_t aIndex);
	void _Update(const opendnp3::Counter& arMeas, size_t aIndex);
	void _Update(const opendnp3::SetpointStatus& arMeas, size_t aIndex);
	void _Update(const opendnp3::ControlStatus& arMeas, size_t aIndex);
	void _End();

	private:

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
