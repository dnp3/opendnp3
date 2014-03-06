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
#ifndef __APDU_HANDLER_BASE_H_
#define __APDU_HANDLER_BASE_H_

#include "opendnp3/app/IINField.h"
#include "opendnp3/app/IAPDUHandler.h"
#include "opendnp3/app/IterableTransforms.h"

#include <openpal/Loggable.h>
#include <openpal/LoggableMacros.h>

namespace opendnp3
{

/**
 * Base class used to handle APDU object headers
 */
	class APDUHandlerBase : public IAPDUHandler, protected openpal::Loggable
	{
	public:

		/**
		 * @param arLogger	the Logger that the loader should use for message reporting
		 */
		APDUHandlerBase(openpal::Logger logger);

		uint32_t NumIgnoredHeaders() const { return ignoredHeaders; }

		IINField Errors() const;

		virtual void AllObjects(GroupVariation gv) final;

		virtual void OnRangeRequest(GroupVariation gv, const StaticRange& range) final;

		virtual void OnCountRequest(GroupVariation gv, uint32_t count) final;

		virtual void OnIIN(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<bool, uint16_t>>& meas) final;

		virtual void OnCountOf(const opendnp3::IterableBuffer<Group50Var1> &) final;		

		virtual void OnCountOf(const opendnp3::IterableBuffer<Group51Var1> &) final;
		virtual void OnCountOf(const opendnp3::IterableBuffer<Group51Var2> &) final;

		virtual void OnCountOf(const opendnp3::IterableBuffer<Group52Var2> &) final;

		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas) final;
		virtual void OnRange(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) final;

		// events - 16bit indices

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas) final;

		// events - 8bit indices

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Binary, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<DoubleBitBinary, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Counter, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<FrozenCounter, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<Analog, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<OctetString, uint8_t>>& meas) final;

		// commands - 16bit indices

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas) final;

		// commands - 8bit indices

		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas) final;
		virtual void OnIndexPrefix(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas) final;


	protected:

		void Reset();
		bool GetCTO(uint64_t& cto);

		inline uint32_t GetCurrentHeader() { return currentHeader; }

		inline bool IsFirstHeader() { return currentHeader == 0; }


		virtual void _AllObjects(GroupVariation gv);

		virtual void _OnRangeRequest(GroupVariation gv, const StaticRange& range);

		virtual void _OnCountRequest(GroupVariation gv, uint32_t count);

		virtual void _OnIIN(const IterableBuffer<IndexedValue<bool, uint16_t>>& meas);

		virtual void _OnCountOf(const IterableBuffer<Group50Var1>& objects);
		virtual void _OnCountOf(const IterableBuffer<Group52Var2> &);

		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);		
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<BinaryOutputStatus, uint16_t>>& meas);
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);		
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<AnalogOutputStatus, uint16_t>>& meas);
		virtual void _OnRange(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);

		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Binary, uint16_t>>& meas);
		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<DoubleBitBinary, uint16_t>>& meas);
		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Counter, uint16_t>>& meas);
		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<FrozenCounter, uint16_t>>& meas);
		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<Analog, uint16_t>>& meas);
		virtual void _OnIndexPrefix(GroupVariation gv, const IterableBuffer<IndexedValue<OctetString, uint16_t>>& meas);

		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint16_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint16_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint16_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint16_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint16_t>>& meas);
		
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<ControlRelayOutputBlock, uint8_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt16, uint8_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputInt32, uint8_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputFloat32, uint8_t>>& meas);
		virtual void _OnIndexPrefix(const IterableBuffer<IndexedValue<AnalogOutputDouble64, uint8_t>>& meas);

	protected:

		uint32_t ignoredHeaders;
		IINField errors;

	private:

		uint64_t cto;
		int32_t ctoHeader;
		int32_t currentHeader;

		template <class T>
		static IndexedValue<T, uint16_t> Widen(const IndexedValue<T, uint8_t> value)
		{
			return value.Widen<uint16_t>();
		}	

		template <class T>
		void OnIndexPrefixCTO(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<T, uint16_t>>& meas);
};

template <class T>
void APDUHandlerBase::OnIndexPrefixCTO(GroupVariation gv, QualifierCode qualifier, const IterableBuffer<IndexedValue<T, uint16_t>>& meas)
{
	uint64_t commonTime;
	if (GetCTO(commonTime))
	{
		auto transform = IterableTransforms<IndexedValue<T, uint16_t>>::Map<IndexedValue<T, uint16_t>>(meas,

			[&commonTime](const IndexedValue<T, uint16_t>& value) {
				T copy(value.value);
				copy.SetTime(commonTime + copy.GetTime());
				return IndexedValue<T, uint16_t>(copy, value.index);
			}

		);
		this->_OnIndexPrefix(gv, transform);	
	}
	else
	{
		LOG_BLOCK(openpal::LogLevel::Warning, "Received CTO objects without preceding common time, using assumed time");
		auto transform = IterableTransforms<IndexedValue<T, uint16_t>>::Map<IndexedValue<T, uint16_t>>(meas,
			[](const IndexedValue<T, uint16_t>& value) {
			T copy(value.value);
			copy.ClearTime();
			return IndexedValue<T, uint16_t>(copy, value.index);
		}
		);
		this->_OnIndexPrefix(gv, transform);
	}
}



}



#endif

