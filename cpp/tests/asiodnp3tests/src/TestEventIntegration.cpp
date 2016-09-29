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

#include <catch.hpp>

#include <opendnp3/LogLevels.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>

#include <asiodnp3/DefaultMasterApplication.h>
#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/ConsoleLogger.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <random>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;

class EventReceiver final : public ISOEHandler
{

public:

	EventReceiver(size_t numToSend, size_t maxOutstanding, std::uint16_t numValues) :
		m_tx_sequence(0),
		m_rx_sequence(0),
		m_num_remaining(numToSend),
		m_num_outstanding(0),
		MAX_OUTSTANDING(maxOutstanding),
		NUM_VALUES(numValues)
	{}

	virtual void Start() override
	{
		m_mutex.lock();
	}
	virtual void End()  override
	{
		m_mutex.unlock();
		m_condition.notify_one();
	}

	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values) override
	{
		auto record = [this](const Indexed<Analog>& item)
		{
			m_rx_values.push_back(item);
		};

		values.ForeachItem(record);
	}

	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values) override {}

	bool LoadAndWait(IOutstation* outstation, std::chrono::steady_clock::duration timeout)
	{

		std::unique_lock<std::mutex> lock(m_mutex);

		const auto RX_NUM = this->ProcessRxValues();

		if (m_num_remaining == 0)
		{
			return true;
		}

		const auto TX_NUM = this->LoadNewValues(outstation);

		if (m_condition.wait_for(lock, timeout) == cv_status::timeout)
		{
			throw std::logic_error("timed out waiting for update");
		}

		return false;
	}

private:

	size_t ProcessRxValues()
	{
		for (auto& value : m_rx_values)
		{
			//std::cout << "received: (" << value.index << ") - " << value.value.value << " qual: " << static_cast<int>(value.value.flags.value) << std::endl;

			if (value.value.value != m_rx_sequence)
			{
				throw std::logic_error("Unexpected rx value");
			}

			if (value.index != (m_rx_sequence % NUM_VALUES))
			{
				throw std::logic_error("Unexpected rx index");
			}

			//std::cout << "done" << std::endl;

			++m_rx_sequence;
		}

		const auto NUM = m_rx_values.size();

		//std::cout << "rx: " << m_rx_values.size() << " out: " << m_num_outstanding << " remain: " << m_num_remaining << std::endl;

		if (NUM > m_num_outstanding)
		{
			throw std::logic_error("rx NUM > m_num_outstanding");
		}

		if (NUM > m_num_remaining)
		{
			throw std::logic_error("rx NUM > m_num_remaining");
		}

		m_num_remaining -= NUM;
		m_num_outstanding -= NUM;
		m_rx_values.clear();
		return NUM;
	}

	size_t LoadNewValues(IOutstation* outstation)
	{
		const auto TX_SAFELY = MAX_OUTSTANDING - m_num_outstanding;
		const auto TX_MAX = TX_SAFELY > m_num_remaining ? m_num_remaining : TX_SAFELY;

		if (TX_MAX == 0)
		{
			return 0;
		}

		// send some random fraction of what's allowed to be sent
		// this makes the test less deterministic and more realistic
		std::uniform_int_distribution<size_t> dis(1, TX_MAX);
		const auto TX_NUM = dis(m_gen);

		ChangeSet changes;

		for (uint32_t i = 0; i < TX_NUM; ++i)
		{
			Analog a(static_cast<double>(m_tx_sequence));
			const uint16_t index = m_tx_sequence % NUM_VALUES;
			changes.Update(a, index);

			//std::cout << "transmitting: (" << index << ") - " << a.value << std::endl;

			++m_tx_sequence;
		}

		outstation->Apply(changes);

		m_num_outstanding += TX_NUM;
		return TX_NUM;
	}

	std::vector<Indexed<Analog>> m_rx_values;

	std::size_t m_tx_sequence;
	std::size_t m_rx_sequence;


	std::size_t m_num_remaining;
	size_t m_num_outstanding;

	const size_t MAX_OUTSTANDING;
	const std::uint16_t NUM_VALUES;

	std::mutex m_mutex;
	std::condition_variable m_condition;


	std::random_device m_random;
	std::mt19937 m_gen;
};

IOutstation* ConfigureOutstation(DNP3Manager& manager, int levels, uint16_t numValues, uint16_t eventBufferSize)
{
	auto server = manager.AddTCPServer("server", levels, ChannelRetry::Default(), "127.0.0.1", 20000, nullptr);

	OutstationStackConfig stackConfig(DatabaseSizes::AllTypes(numValues));
	stackConfig.outstation.eventBufferConfig = EventBufferConfig::AllTypes(eventBufferSize);
	stackConfig.outstation.params.allowUnsolicited = true;

	auto outstation = server->AddOutstation("outstation", SuccessCommandHandler::Create(), DefaultOutstationApplication::Create(), stackConfig);
	outstation->Enable();
	return outstation;
}

IMaster* ConfigureMaster(DNP3Manager& manager, std::shared_ptr<ISOEHandler> handler, int levels)
{
	auto client = manager.AddTCPClient("client", levels, ChannelRetry::Default(), "127.0.0.1", "127.0.0.1", 20000, nullptr);

	MasterStackConfig mconfig;
	mconfig.master.startupIntegrityClassMask = ClassField::None(); //disable integrity poll so we don't have to worry about static values coming back
	auto master = client->AddMaster("master", handler, DefaultMasterApplication::Create(), mconfig);
	master->Enable();
	return master;
}

#define SUITE(name) "EventIntegrationTestSuite - " name

TEST_CASE(SUITE("TestEventIntegration"))
{
	const auto LEVELS = levels::NORMAL | flags::APP_HEADER_RX | flags::APP_HEADER_TX | flags::APP_OBJECT_RX | flags::APP_OBJECT_TX;

	const uint32_t NUM_TO_SEND = 100000;						// send 100,000 values
	const uint16_t EVENT_BUFFER_SIZE = 100;						// size of the events buffers for all types
	const uint32_t MAX_OUTSTANDING = EVENT_BUFFER_SIZE / 4;		// important to keep this below the event buffer size, otherwise we can overflow it and lose events
	const uint16_t NUM_VALUES = 100;							// size of the static database. we'll rotate through indices


	auto eventrx = std::make_shared<EventReceiver>(NUM_TO_SEND, MAX_OUTSTANDING, NUM_VALUES);

	DNP3Manager manager(2);// , ConsoleLogger::Create());

	auto outstation = ConfigureOutstation(manager, LEVELS, NUM_VALUES, EVENT_BUFFER_SIZE);
	auto master = ConfigureMaster(manager, eventrx, LEVELS);

	while (!eventrx->LoadAndWait(outstation, std::chrono::seconds(3)));
}

