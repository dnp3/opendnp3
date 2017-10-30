/*
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

#include "opendnp3/outstation/EventWriteHandler.h"
#include "opendnp3/app/EventType.h"
#include "opendnp3/app/MeasurementTypeSpecs.h"

#include <vector>
#include <deque>
#include <sstream>

namespace opendnp3
{
	class MockEventWriteHandler final : public EventWriteHandler
	{
		struct Record
		{
			EventType type;
			uint8_t variation;
			uint16_t count;
		};

		std::deque<Record> expected;

	private:

		template <class T>
		class EventWriterImpl : public EventWriter<T>
		{
		public:

			bool success = true;
			std::vector<uint16_t> written_indices;

			virtual bool Write(const T& meas, uint16_t index) override
			{
				if (success) {
					written_indices.push_back(index);
				}
				return success;
			}
		};


	public:

		void Expect(EventBinaryVariation variation, uint16_t count)
		{
			expected.push_back(
				Record { EventType::Binary, static_cast<uint8_t>(variation), count }
			);
		}

		void AssertEmpty()
		{
			if (!this->expected.empty())
			{
				throw std::logic_error("more events expected");
			}
		}


		virtual void Write(EventBinaryVariation variation, EventCollection<Binary>& items) override
		{
			this->WriteAny<BinarySpec>(variation, items);
		}

		virtual void Write(EventDoubleBinaryVariation variation, EventCollection<DoubleBitBinary>& items) override
		{
			this->WriteAny<DoubleBitBinarySpec>(variation, items);
		}
		
		virtual void Write(EventCounterVariation variation, EventCollection<Counter>& items) override
		{
			this->WriteAny<CounterSpec>(variation, items);
		}

		virtual void Write(EventFrozenCounterVariation variation, EventCollection<FrozenCounter>& items) override
		{
			this->WriteAny<FrozenCounterSpec>(variation, items);
		}

		virtual void Write(EventAnalogVariation variation, EventCollection<Analog>& items) override
		{
			this->WriteAny<AnalogSpec>(variation, items);
		}

		virtual void Write(EventBinaryOutputStatusVariation variation, EventCollection<BinaryOutputStatus>& items) override
		{
			this->WriteAny<BinaryOutputStatusSpec>(variation, items);
		}

		virtual void Write(EventAnalogOutputStatusVariation variation, EventCollection<AnalogOutputStatus>& items) override
		{
			this->WriteAny<AnalogOutputStatusSpec>(variation, items);
		}

	private:

		template <class T>
		void WriteAny(typename T::event_variation_t variation, EventCollection<typename T::meas_t>& items);
		
	};

	template <class T>
	void MockEventWriteHandler::WriteAny(typename T::event_variation_t variation, EventCollection<typename T::meas_t>& items)
	{
		if (this->expected.empty()) {
			throw std::logic_error("no more write events expected");
		}

		const auto record = this->expected.front();
		this->expected.pop_front();

		if (record.type != EventType::Binary) {
			std::ostringstream oss;
			oss << "Unexpected event type: " << static_cast<int>(record.type);
			throw std::logic_error(oss.str());
		}

		if (record.variation != static_cast<uint8_t>(variation))
		{
			std::ostringstream oss;
			oss << "Unexpected variation: " << static_cast<int>(variation);
			throw std::logic_error(oss.str());
		}

		EventWriterImpl<typename T::meas_t> writer;
		const uint16_t count = items.WriteSome(writer);

		if (record.count != count) {
			std::ostringstream oss;
			oss << "Unexpected count: " << static_cast<int>(variation);
			throw std::logic_error(oss.str());
		}
	}
}

