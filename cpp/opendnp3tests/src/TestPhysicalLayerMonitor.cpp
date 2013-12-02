
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the terms of the License.
//

#include <boost/test/unit_test.hpp>

#include <openpal/Exception.h>

#include <asiopal/Log.h>

#include <opendnp3/PhysicalLayerMonitor.h>
#include <opendnp3/PhysicalLayerMonitorStates.h>

#include "MockExecutor.h"
#include "MockPhysicalLayerAsync.h"
#include "TestHelpers.h"

using namespace opendnp3;
using namespace std::chrono;
using namespace openpal;
using namespace asiopal;

class ConcretePhysicalLayerMonitor : public PhysicalLayerMonitor
{
public:

	ConcretePhysicalLayerMonitor(openpal::Logger aLogger, IPhysicalLayerAsync* apPhys) :
		Loggable(aLogger),
		PhysicalLayerMonitor(aLogger.GetSubLogger("monitor"), apPhys, TimeDuration::Seconds(1),  TimeDuration::Seconds(10)),
		mOpenCallbackCount(0),
		mCloseCallbackCount(0) {
	}

	void ReachInAndStartOpenTimer() {
		this->StartOpenTimer();
	}

	size_t mOpenCallbackCount;
	size_t mCloseCallbackCount;

protected:

	void OnPhysicalLayerOpenSuccessCallback() {
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerOpenFailureCallback() {
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerCloseCallback() {
		++mCloseCallbackCount;
	}

	void _OnReceive(const openpal::ReadOnlyBuffer&) {}
	void _OnSendSuccess() {}
	void _OnSendFailure() {}
};

class TestObject
{
public:

	TestObject() :
		log(),
		exe(),
		phys(Logger(&log, LogLevel::Info, "mock-phys"), &exe),
		monitor(Logger(&log, LogLevel::Info, "test"), &phys)
	{}

	EventLog log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	ConcretePhysicalLayerMonitor monitor;
};

BOOST_AUTO_TEST_SUITE(PhysicalLayerMonitorTestSuite)

BOOST_AUTO_TEST_CASE(StateClosedExceptions)
{
	TestObject test;
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
	BOOST_REQUIRE_THROW(test.monitor.OnLowerLayerUp(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.monitor.OnLowerLayerDown(), InvalidStateException);
	BOOST_REQUIRE_THROW(test.monitor.OnOpenFailure(), InvalidStateException);
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(ThrowsIfEverNotExpectingOpenTimer)
{
	TestObject test;
	test.monitor.ReachInAndStartOpenTimer();
	BOOST_REQUIRE_THROW(test.exe.DispatchOne(), InvalidStateException);
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StateClosedCanBeStopped)
{
	TestObject test;
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StateClosedIgnoresSuspend)
{
	TestObject test;
	test.monitor.Suspend();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StartOneBeginsOpening)
{
	TestObject test;
	test.monitor.StartOne();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StartOneFailureDoesNotRetry)
{
	TestObject test;
	test.monitor.StartOne();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StartingWhileOpeningOneWillRetry)
{
	TestObject test;
	test.monitor.StartOne();
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(ClosingWhileOpeningOneWillNotRetry)
{
	TestObject test;
	test.monitor.StartOne();
	test.monitor.Close();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(SuspendWhileOpeningClosingWillNotRetry)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	test.monitor.Suspend();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(LayerCloseAfterStartOneDoesNotRetry)
{
	TestObject test;
	test.monitor.StartOne();
	test.phys.SignalOpenSuccess();
	BOOST_REQUIRE(ChannelState::OPEN == test.monitor.GetState());
	test.phys.AsyncClose();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StopAndCloseDoNothingWhileStopped)
{
	TestObject test;
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());

	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
	test.monitor.Close();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(StoppedLayerCannotBeStarted)
{
	TestObject test;
	test.monitor.Shutdown();
	test.monitor.Start();
	test.monitor.StartOne();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
	BOOST_REQUIRE(test.phys.IsClosed());
}

BOOST_AUTO_TEST_CASE(ClosedLayerCanBeStarted)
{
	TestObject test;
	test.monitor.Start();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	BOOST_REQUIRE(test.phys.IsOpening());
}

BOOST_AUTO_TEST_CASE(OpeningLayerExceptions)
{
	TestObject test;
	test.monitor.Start();
	BOOST_REQUIRE_THROW(test.monitor.OnLowerLayerDown(), InvalidStateException);
}

BOOST_AUTO_TEST_CASE(OpeningStartOneGoesToOpeningOne)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.StartOne();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(OpeningStateOpenSuccessGoesToOpenState)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	BOOST_REQUIRE(ChannelState::OPEN == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(1, test.monitor.mOpenCallbackCount); //check that the callback fired
}

BOOST_AUTO_TEST_CASE(StopWhileOpeningWaitsForOpenFailure)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(CloseWhileOpeningWaitsForOpenFailureThenWaits)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(CloseWhileOpeningAndThenStop)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(OpenFailureGoesToWaitingAndExponentialBackoff)
{
	TestObject test;
	test.monitor.Start();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(1, test.exe.NumActive());
	BOOST_REQUIRE_EQUAL(1000, test.exe.NextTimerExpiration().milliseconds);
	BOOST_REQUIRE(test.exe.DispatchOne());
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(1, test.exe.NumActive());
	BOOST_REQUIRE_EQUAL(2000, test.exe.NextTimerExpiration().milliseconds);
}

BOOST_AUTO_TEST_CASE(OpenFailureGoesToClosedIfSuspended)
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Suspend();
	test.phys.SignalOpenFailure();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(0, test.exe.NumActive());
}

BOOST_AUTO_TEST_CASE(ShutdownPostsToTimer)
{
	TestObject test;
	BOOST_REQUIRE_EQUAL(0, test.exe.NumActive());
	BOOST_REQUIRE_FALSE(test.monitor.WaitForShutdown(TimeDuration::Seconds(0)));
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(1, test.exe.NumActive());
	BOOST_REQUIRE_FALSE(test.monitor.WaitForShutdown(TimeDuration::Seconds(0)));
	BOOST_REQUIRE(test.exe.DispatchOne());
	BOOST_REQUIRE(test.monitor.WaitForShutdown()); //wait indefinitely, but it's already shutdown
}

BOOST_AUTO_TEST_CASE(ShutdownWhileWaitingCancelsTimer)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Shutdown();
	BOOST_REQUIRE(test.exe.DispatchOne()); //disptach the shutdown post
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(0, test.exe.NumActive());
}

BOOST_AUTO_TEST_CASE(LayerKeepsTryingToOpen)
{
	TestObject test;
	test.monitor.Start();

	for(size_t i = 0; i < 3; ++i) {
		BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
		test.phys.SignalOpenFailure();
		BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
		BOOST_REQUIRE(test.exe.DispatchOne());
	}
}

BOOST_AUTO_TEST_CASE(CloseWhileWaitingDoesNothing)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Close();
	BOOST_REQUIRE(ChannelState::WAITING == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(1, test.exe.NumActive());
}

BOOST_AUTO_TEST_CASE(LayerCloseWhileOpen)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.phys.AsyncClose();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(RequestCloseWhileOpen)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Close();
	BOOST_REQUIRE(ChannelState::OPENING == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(RequestStopWhileOpen)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Shutdown();
	BOOST_REQUIRE(ChannelState::SHUTDOWN == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(LayerSuspendWhileOpenDontRetry)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Suspend();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
}

BOOST_AUTO_TEST_CASE(SuspendWhileWaitingCancelsTimer)
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Suspend();
	BOOST_REQUIRE(ChannelState::CLOSED == test.monitor.GetState());
	BOOST_REQUIRE_EQUAL(0, test.exe.NumActive());
}

BOOST_AUTO_TEST_SUITE_END()
