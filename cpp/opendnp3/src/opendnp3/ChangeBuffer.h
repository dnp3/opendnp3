/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef __CHANGE_BUFFER_H_
#define __CHANGE_BUFFER_H_

#include <opendnp3/DataTypes.h>
#include <opendnp3/IDataObserver.h>
#include <opendnp3/SubjectBase.h>


#include <queue>
#include <mutex>

namespace opendnp3
{

/** Moves measurement data across thread boundaries.
*/
class ChangeBuffer : public IDataObserver, public SubjectBase
{

public:

	ChangeBuffer() : mNotify(false) {}
	
	void Update(const Binary& arPoint, uint16_t aIndex)  final;
	void Update(const Analog& arPoint, uint16_t aIndex) final;
	void Update(const Counter& arPoint, uint16_t aIndex) final;
	void Update(const ControlStatus& arPoint, uint16_t aIndex) final;
	void Update(const SetpointStatus& arPoint, uint16_t aIndex) final;

	size_t FlushUpdates(IDataObserver* apObserver);

protected:

	void Start() final;
	void End() final;

private:

	void Clear();

	template <class T>
	static void Dispatch(IDataObserver* apObs, T& arMeas, uint16_t aIndex) {
		apObs->Update(arMeas, aIndex);
	}


	void _Clear();

	bool mNotify;

	std::deque<std::function<void (IDataObserver*)>> mChangeQueue;
	std::mutex mMutex;
};

}

#endif

