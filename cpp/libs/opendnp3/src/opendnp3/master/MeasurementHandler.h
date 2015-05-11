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
#ifndef OPENDNP3_MEASUREMENTHANDLER_H
#define OPENDNP3_MEASUREMENTHANDLER_H

#include <openpal/logging/Logger.h>

#include "opendnp3/master/ISOEHandler.h"

#include "opendnp3/app/APDUHeader.h"
#include "opendnp3/app/parsing/ParseResult.h"
#include "opendnp3/app/parsing/APDUHandlerBase.h"

namespace opendnp3
{

/**
 * Dedicated class for processing response data in the master.
 */
class MeasurementHandler : public APDUHandlerBase
{

public:

	/**
	* Static helper function for interpreting a response as a measurement response	
	*/
	static ParseResult ProcessMeasurements(const openpal::ReadBufferView& objects, openpal::Logger& logger, ISOEHandler* pHandler);

	/**
	* Creates a new ResponseLoader instance.
	*
	* @param logger	the Logger that the loader should use for message reporting
	*/
	MeasurementHandler(const openpal::Logger& logger, ISOEHandler* pSOEHandler);	

	~MeasurementHandler();

private:

	static TimestampMode ModeFromType(GroupVariation gv);

	// Handle the CTO objects
	IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var1& cto) override final;
	IINField ProcessCount(const HeaderRecord& record, uint16_t pos, uint16_t total, const Group51Var2& cto) override final;

	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index)  final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const TimeAndInterval& meas, uint16_t index) override final;
	IINField ProcessRange(const HeaderRecord& record, uint32_t count, const Group121Var1& meas, uint16_t index) override final;

	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Binary& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryOutputStatus& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const DoubleBitBinary& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Counter& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const FrozenCounter& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Analog& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogOutputStatus& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const OctetString& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const BinaryCommandEvent& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const AnalogCommandEvent& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var1& meas, uint16_t index) override final;
	IINField ProcessIndexPrefix(const HeaderRecord& record, uint32_t count, const Group122Var2& meas, uint16_t index) override final;

	template <class T>
	IINField LoadAny(const HeaderRecord& record, TimestampMode tsmode, const ICollection<IndexedValue<T, uint16_t>>& meas)
	{		
		this->CheckForTxStart();
		HeaderInfo info(record.enumeration, record.GetQualifierCode(), tsmode, record.headerCount);
		auto iterate = [info, this](const IndexedValue<T, uint16_t>& pair) 
		{
			this->pSOEHandler->OnValue(info, pair.value, pair.index);			
		};
		meas.foreach(iterate);		
		return IINField();
	}

	template <class T>
	IINField LoadSingleValue(const HeaderRecord& record, TimestampMode tsmode, const T& meas, uint16_t index)
	{
		this->CheckForTxStart();
		HeaderInfo info(record.enumeration, record.GetQualifierCode(), tsmode, record.headerCount);
		this->pSOEHandler->OnValue(info, meas, index);
		return IINField();
	}

	template <class T>
	IINField ProcessWithCTO(const HeaderRecord& record, const T& meas, uint16_t index);

	bool txInitiated;
	ISOEHandler* pSOEHandler;

	TimestampMode ctoMode;
	uint32_t ctoHeader;
	uint64_t commonTimeOccurence;

	void CheckForTxStart();

};

template <class T>
IINField MeasurementHandler::ProcessWithCTO(const HeaderRecord& record, const T& meas, uint16_t index)
{	
	if (ctoMode == TimestampMode::INVALID || ((ctoHeader + 1) != GetCurrentHeader()))
	{
		FORMAT_LOG_BLOCK(logger, flags::WARN, "No prior CTO objects for %s", GroupVariationToString(record.enumeration));
		return IINField(IINBit::PARAM_ERROR);
	}

	const auto MODE = this->ctoMode;
	
	T copy(meas);
	copy.time = DNPTime(meas.time + this->commonTimeOccurence);	

	return this->LoadSingleValue(record, MODE, copy, index);
}

}



#endif

