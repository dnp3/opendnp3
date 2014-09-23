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
#ifndef OPENDNP3_IEVENTBUFFER_H
#define OPENDNP3_IEVENTBUFFER_H

#include "opendnp3/app/MeasurementTypes.h"

#include "opendnp3/outstation/Event.h"

namespace opendnp3
{

// @section desc Used by the database
class IEventBuffer
{
public:

	virtual ~IEventBuffer() {}

	virtual void Update(const Event<Binary>& evt) = 0;

	virtual void Update(const Event<DoubleBitBinary>& evt) = 0;

	virtual void Update(const Event<Analog>& evt) = 0;

	virtual void Update(const Event<Counter>&  evt) = 0;

	virtual void Update(const Event<FrozenCounter>&  evt) = 0;

	virtual void Update(const Event<BinaryOutputStatus>& evt) = 0;

	virtual void Update(const Event<AnalogOutputStatus>& evt) = 0;

};

}


#endif

