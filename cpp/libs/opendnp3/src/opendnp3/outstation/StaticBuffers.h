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
#ifndef OPENDNP3_STATICBUFFERS_H
#define OPENDNP3_STATICBUFFERS_H

#include "DatabaseConfigView.h"

#include "opendnp3/outstation/Cell.h"
#include "opendnp3/outstation/DatabaseTemplate.h"

#include <openpal/container/Array.h>
#include <openpal/util/Uncopyable.h>

namespace opendnp3
{

/**
* The static database provides storage for current values and all of the associated metadata
*/
class StaticBuffers : private openpal::Uncopyable
{

public:

	StaticBuffers(const DatabaseTemplate& dbTemplate);

	DatabaseConfigView GetView() const;

	// specializations in cpp file
	template <class T>
	openpal::ArrayView<Cell<T>, uint16_t> GetArrayView();

private:

	template <class T>
	void SetDefaultIndices()
	{
		auto view = GetArrayView<T>();
		for (uint16_t i = 0; i < view.Size(); ++i)
		{
			view[i].vIndex = i;
		}
	}
		
	openpal::Array<Cell<Binary>, uint16_t> binaries;
	openpal::Array<Cell<DoubleBitBinary>, uint16_t> doubleBinaries;
	openpal::Array<Cell<Analog>, uint16_t> analogs;
	openpal::Array<Cell<Counter>, uint16_t> counters;
	openpal::Array<Cell<FrozenCounter>, uint16_t> frozenCounters;
	openpal::Array<Cell<BinaryOutputStatus>, uint16_t> binaryOutputStatii;
	openpal::Array<Cell<AnalogOutputStatus>, uint16_t> analogOutputStatii;
	openpal::Array<Cell<TimeAndInterval>, uint16_t> timeAndIntervals;
};

}

#endif
