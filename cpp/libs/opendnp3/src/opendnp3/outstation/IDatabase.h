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
#ifndef OPENDNP3_IDATABASE_H
#define OPENDNP3_IDATABASE_H

#include "opendnp3/app/ITransactable.h"
#include "opendnp3/app/MeasurementTypes.h"


namespace opendnp3
{

/**
@desc IDatabase is the interface used to load measurements into an outstation.

The outstation application is responsible for measuring or
calculating data and then pushing it into this interface when it is ready
to publish it. As with all ITransactable objects it should be used with the
exception-safe Transaction object.

\code
{
	Transaction t(pDatabase);
	pLoader->Update(Analog(12), 3);
	pLoader->Update(Binary(true), 1);
}
\endcode

*/
class IDatabase : public ITransactable
{
public:

	virtual ~IDatabase() {}

	/**
	* Update a Binary measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const Binary& meas, uint16_t index) = 0;

	/**
	* Update a DoubleBitBinary measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const DoubleBitBinary& meas, uint16_t index) = 0;

	/**
	* Update an Analog measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const Analog& meas, uint16_t index) = 0;

	/**
	* Update a Counter measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const Counter& meas, uint16_t index) = 0;

	/**
	* Update a FrozenCounter measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const FrozenCounter& meas, uint16_t index) = 0;

	/**
	* Update a BinaryOutputStatus measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const BinaryOutputStatus& meas, uint16_t index) = 0;

	/**
	* Update a AnalogOutputStatus measurement, must have transaction started
	* @param meas measurement to be processed
	* @param index index of the measurement
	*/
	virtual void Update(const AnalogOutputStatus& meas, uint16_t index) = 0;

};

}

#endif
