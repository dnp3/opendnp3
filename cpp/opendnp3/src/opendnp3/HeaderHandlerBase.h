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
#ifndef __HEADER_HANDLER_BASE_H_
#define __HEADER_HANDLER_BASE_H_

#include <openpal/Loggable.h>

#include "IAPDUHeaderHandler.h"

namespace opendnp3
{

/**
 * Base class used to handle APDU object headers
 */
class HeaderHandlerBase : public openpal::Loggable, public IAPDUHeaderHandler
{
public:

	/**	 	 
	 * @param arLogger	the Logger that the loader should use for message reporting
	 */
	HeaderHandlerBase(openpal::Logger& arLogger);	

	virtual void AllObjects(GroupVariation gv, const openpal::ReadOnlyBuffer& header) final;
	virtual void OnIIN(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<bool>>& meas) final;

	virtual void OnCountOf(const opendnp3::Iterable<Group52Var2> &) final;

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Binary>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Binary>>& meas) final;

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<ControlStatus>>& meas) final;
		
	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Counter>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Counter>>& meas) final;

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Analog>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<Analog>>& meas) final;

	virtual void OnRange(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<SetpointStatus>>& meas) final;

	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<ControlRelayOutputBlock>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputInt16>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputInt32>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputFloat32>>& meas) final;
	virtual void OnIndexPrefix(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputDouble64>>& meas) final;

	virtual void OnRangeOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) final;
	virtual void OnIndexPrefixOfOctets(GroupVariation gv, const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas) final;

	protected:

	virtual std::string HandlerName() const = 0;

	bool GetCTO(int64_t& cto);

	virtual void _AllObjects(GroupVariation gv);
	virtual void _OnIIN(const Iterable<IndexedValue<bool>>& meas);

	virtual void _OnCountOf(const opendnp3::Iterable<Group52Var2> &);

	virtual void _OnRange(GroupVariation gv, const Iterable<IndexedValue<Binary>>& meas);
	virtual void _OnIndexPrefix(GroupVariation gv, const Iterable<IndexedValue<Binary>>& meas);

	virtual void _OnRange(GroupVariation gv, const Iterable<IndexedValue<ControlStatus>>& meas);
		
	virtual void _OnRange(GroupVariation gv, const Iterable<IndexedValue<Counter>>& meas);
	virtual void _OnIndexPrefix(GroupVariation gv, const Iterable<IndexedValue<Counter>>& meas);

	virtual void _OnRange(GroupVariation gv, const Iterable<IndexedValue<Analog>>& meas);
	virtual void _OnIndexPrefix(GroupVariation gv, const Iterable<IndexedValue<Analog>>& meas);

	virtual void _OnRange(GroupVariation gv, const Iterable<IndexedValue<SetpointStatus>>& meas) ;

	virtual void _OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<ControlRelayOutputBlock>>& meas);

	virtual void _OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputInt16>>& meas);
	virtual void _OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputInt32>>& meas);
	virtual void _OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputFloat32>>& meas);
	virtual void _OnIndexPrefix(const openpal::ReadOnlyBuffer& header, const Iterable<IndexedValue<AnalogOutputDouble64>>& meas);

	virtual void _OnRangeOfOctets(GroupVariation gv, const Iterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas);
	virtual void _OnIndexPrefixOfOctets(GroupVariation gv, const Iterable<IndexedValue<openpal::ReadOnlyBuffer>>& meas);


	private:

	int32_t currentHeader;
	int64_t cto;
	int32_t ctoHeader;
};

}

/* vim: set ts=4 sw=4: */

#endif

