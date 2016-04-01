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
#include <asiodnp3/MeasUpdate.h>

#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;

class EventReceiver final : public ISOEHandler {

public:

	EventReceiver() : m_sequence(0)
	{}

	virtual void Start() override { m_mutex.lock(); }
	virtual void End()  override { 
		m_mutex.unlock();
		m_condition.notify_one();
	}
	
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Binary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<DoubleBitBinary>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Analog>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<Counter>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<FrozenCounter>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryOutputStatus>>& values) override {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogOutputStatus>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<OctetString>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<TimeAndInterval>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<BinaryCommandEvent>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<AnalogCommandEvent>>& values) override  {}
	virtual void Process(const HeaderInfo& info, const ICollection<Indexed<SecurityStat>>& values) override {}

	bool LoadAndWait(std::chrono::monotonic_clock::duration timeout) {
		std::unique_lock<std::mutex> lock(m_mutex);



		return m_condition.wait_for(lock, timeout) != cv_status::timeout;
	}

private:

	std::vector<Indexed<Analog>> m_values;

	std::uint32_t m_sequence;
	std::mutex m_mutex;
	std::condition_variable m_condition;
};

IOutstation* ConfigureOutstation(DNP3Manager& manager, int levels)
{
	auto server = manager.AddTCPServer("server", levels, ChannelRetry::Default(), "127.0.0.1", 20000);

	OutstationStackConfig stackConfig;	
	stackConfig.dbTemplate = DatabaseTemplate::AllTypes(100);
	stackConfig.outstation.eventBufferConfig = EventBufferConfig::AllTypes(100);
	stackConfig.outstation.params.allowUnsolicited = true;

	auto outstation = server->AddOutstation("outstation", SuccessCommandHandler::Instance(), DefaultOutstationApplication::Instance(), stackConfig);
	outstation->Enable();
	return outstation;
}

IMaster* ConfigureMaster(DNP3Manager& manager, ISOEHandler& handler, int levels)
{
	auto client = manager.AddTCPClient("client", levels, ChannelRetry::Default(), "127.0.0.1", "127.0.0.1", 20000);

	MasterStackConfig mconfig;
	mconfig.master.startupIntegrityClassMask = ClassField::None(); //disable integrity poll so we don't have to worry about static values coming back
	auto master = client->AddMaster("master", handler, DefaultMasterApplication::Instance(), mconfig);
	master->Enable();
	return master;
}

#define SUITE(name) "EventIntegrationTestSuite - " name

TEST_CASE(SUITE("TestEventIntegration"))
{
	const auto LEVELS = levels::NORMAL | flags::APP_HEADER_RX | flags::APP_HEADER_TX | flags::APP_OBJECT_RX | flags::APP_OBJECT_TX;

	EventReceiver eventrx;

	DNP3Manager manager(2);
	manager.AddLogSubscriber(ConsoleLogger::Instance());		

	auto outstation = ConfigureOutstation(manager, LEVELS);
	auto master = ConfigureMaster(manager, eventrx, LEVELS);

	for(int i=0; i<100; ++i) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		MeasUpdate tx(outstation);
		tx.Update(Analog(i), 0);
	}
}

