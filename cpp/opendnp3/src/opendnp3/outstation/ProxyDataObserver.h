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
#ifndef __PROXY_DATA_OBSERVER_H_
#define __PROXY_DATA_OBSERVER__H_

#include "opendnp3/app/MeasurementTypes.h"
#include "opendnp3/outstation/IDataObserver.h"

namespace openpal
{
class IExecutor;
}

namespace opendnp3
{

/** Moves measurement data across thread boundaries.
*/
class ProxyDataObserver : public IDataObserver
{

public:

	ProxyDataObserver(IDataObserver* pProxy_, openpal::IExecutor* pExecutor_);

	void Update(const Binary& meas, uint16_t index)  final;
	void Update(const Analog& meas, uint16_t index) final;
	void Update(const Counter& meas, uint16_t index) final;
	void Update(const FrozenCounter& meas, uint16_t index) final;
	void Update(const BinaryOutputStatus& meas, uint16_t index) final;
	void Update(const AnalogOutputStatus& meas, uint16_t index) final;

protected:

	void Start() final;
	void End() final;

private:

	openpal::IExecutor* pExecutor;
	IDataObserver* pProxy;
};

}

#endif

