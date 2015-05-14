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
#ifndef OPENDNP3_APDUHANDLERBASE_H
#define OPENDNP3_APDUHANDLERBASE_H

#include "opendnp3/app/IINField.h"

#include "opendnp3/app/parsing/IAPDUHandler.h"
#include "opendnp3/LogLevels.h"

#include <openpal/logging/LogMacros.h>
#include <openpal/logging/Logger.h>



namespace opendnp3
{

/**
 * Base class used to handle APDU object headers
 */
class APDUHandlerBase : public IAPDUHandler
{
public:

	/**
	 * @param logger the Logger that the loader should use for message reporting
	 */
	APDUHandlerBase();

	uint32_t NumIgnoredHeaders() const
	{
		return numIgnoredHeaders;
	}

	IINField Errors() const;

	virtual void OnHeader(const AllObjectsHeader& header) override final;

	virtual void OnHeader(const RangeHeader& header) override final;

	virtual void OnHeader(const CountHeader& header) override final;

	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var1& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var2& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var5& value) override final;
	virtual void OnFreeFormat(const HeaderRecord& record, const Group120Var6& value) override final;

	virtual void OnValues(const CountHeader& header, const ICollection<Group50Var1>& values) override final;
	virtual void OnValues(const CountHeader& header, const ICollection<Group51Var1>& values) override final;
	virtual void OnValues(const CountHeader& header, const ICollection<Group51Var2>& values) override final;
	virtual void OnValues(const CountHeader& header, const ICollection<Group52Var2>& values) override final;
	virtual void OnValues(const CountHeader& header, const ICollection<Group120Var4>& values) override final;

	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<IINValue>> &values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<Binary>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<Counter>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<Analog>>& values)  override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override final;
	virtual void OnValues(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values) override final;

	// events

	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values) override final;

	// commands

	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values) override final;
	virtual void OnValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values) override final;

protected:

	void Reset();	

	inline uint32_t GetCurrentHeader()
	{
		return numTotalHeaders;
	}

	inline bool IsFirstHeader()
	{
		return numTotalHeaders == 0;
	}


	virtual IINField ProcessHeader(const AllObjectsHeader& record);
	virtual IINField ProcessHeader(const RangeHeader& header);
	virtual IINField ProcessHeader(const CountHeader& header);

	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var1& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var2& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var5& value);
	virtual IINField ProcessFreeFormat(const HeaderRecord& record, const Group120Var6& value);
	
	virtual IINField ProcessValues(const CountHeader& header, const ICollection<Group50Var1>& values);
	virtual IINField ProcessValues(const CountHeader& header, const ICollection<Group51Var1>& values);
	virtual IINField ProcessValues(const CountHeader& header, const ICollection<Group51Var2>& values);
	virtual IINField ProcessValues(const CountHeader& header, const ICollection<Group52Var2>& values);
	virtual IINField ProcessValues(const CountHeader& header, const ICollection<Group120Var4>& values);
	
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<IINValue>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<Binary>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<Counter>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<FrozenCounter>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<Analog>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<OctetString>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<TimeAndInterval>>& values);
	virtual IINField ProcessValues(const RangeHeader& header, const ICollection<Indexed<Group121Var1>>& values);

	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Binary>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<BinaryOutputStatus>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<DoubleBitBinary>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Counter>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<FrozenCounter>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Analog>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputStatus>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<OctetString>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<TimeAndInterval>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<BinaryCommandEvent>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogCommandEvent>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var1>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<Group122Var2>>& values);

	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<ControlRelayOutputBlock>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt16>> &values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputInt32>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputFloat32>>& values);
	virtual IINField ProcessValues(const PrefixHeader& header, const ICollection<Indexed<AnalogOutputDouble64>>& values);

protected:	

	// overridable to receive events for every header
	virtual void OnHeaderResult(const HeaderRecord& record, const IINField& result) {}
	
private:

	inline void Record(const HeaderRecord& record, const IINField& result)
	{
		errors |= result;
		++numTotalHeaders;
		this->OnHeaderResult(record, result);
	}

	inline IINField ProcessUnsupportedHeader()
	{
		++numIgnoredHeaders;
		return IINField(IINBit::FUNC_NOT_SUPPORTED);
	}

	IINField errors;
	uint32_t numTotalHeaders;
	uint32_t numIgnoredHeaders;			
};

}



#endif

