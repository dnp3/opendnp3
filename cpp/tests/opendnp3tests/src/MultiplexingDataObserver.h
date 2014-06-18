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
#ifndef __MULTIPLEXING_DATA_OBSERVER_H_
#define __MULTIPLEXING_DATA_OBSERVER_H_

#include <opendnp3/outstation/IMeasurementLoader.h>

#include <vector>

namespace opendnp3
{

/** DataObserver for sending updates to multiple data observers.
*/
class MultiplexingDataObserver : public IMeasurementLoader
{
public:

	MultiplexingDataObserver();
	MultiplexingDataObserver(IMeasurementLoader* apObserver1);
	MultiplexingDataObserver(IMeasurementLoader* apObserver1, IMeasurementLoader* apObserver2);

	void AddObserver(IMeasurementLoader* apObserver1);

	void Update(const Binary& arPoint, uint16_t aIndex);
	void Update(const Analog& arPoint, uint16_t aIndex);
	void Update(const Counter& arPoint, uint16_t aIndex);
	void Update(const BinaryOutputStatus& arPoint, uint16_t aIndex);
	void Update(const AnalogOutputStatus& arPoint, uint16_t aIndex);

private:

	std::vector<IMeasurementLoader*> mObservers;

	void Start();
	void End();
};

}

#endif

