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

#include <asiodnp3/PhysicalLayerMonitor.h>

#include "mocks/MockPhysicalLayer.h"

#include <testlib/MockExecutor.h>
#include <testlib/MockLogHandler.h>

using namespace opendnp3;
using namespace openpal;

class ConcretePhysicalLayerMonitor final : public asiodnp3::PhysicalLayerMonitor
{
public:

	ConcretePhysicalLayerMonitor(openpal::Logger logger, openpal::IExecutor& executor, IPhysicalLayer& phys) :
		PhysicalLayerMonitor(logger, executor, &phys, ChannelRetry::Default()),
		mOpenCallbackCount(0),
		mCloseCallbackCount(0),
		mShutdownCallbackCount(0)
	{

	}

	virtual void OnReceive(const openpal::RSlice&) override {}
	virtual void OnSendResult(bool isSuccess) override {}

	void ReachInAndStartOpenTimer()
	{
		this->StartOpenTimer();
	}

	size_t mOpenCallbackCount;
	size_t mCloseCallbackCount;
	size_t mShutdownCallbackCount;

protected:

	void OnShutdown() override
	{
		++mShutdownCallbackCount;
	}

	void OnPhysicalLayerOpenSuccessCallback() override
	{
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerOpenFailureCallback() override
	{
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerCloseCallback() override
	{
		++mCloseCallbackCount;
	}

};

class TestObject
{
public:

	TestObject(uint32_t filters = levels::NORMAL) :
		log(),
		exe(),
		phys(log.root.logger, exe),
		monitor(log.root.logger, exe, phys)
	{}

	testlib::MockLogHandler log;
	testlib::MockExecutor exe;
	MockPhysicalLayer phys;
	ConcretePhysicalLayerMonitor monitor;
};

#define SUITE(name) "PhysicalLayerMonitorTestSuite - " name

TEST_CASE(SUITE("StateClosed"))
{
	TestObject test;
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));

	test.monitor.OnLowerLayerUp();
	REQUIRE(test.log.PopOneEntry(flags::ERR));
	test.monitor.OnLowerLayerDown();
	REQUIRE(test.log.PopOneEntry(flags::ERR));
	test.monitor.OnOpenFailure();
	REQUIRE(test.log.PopOneEntry(flags::ERR));


	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("ThrowsIfEverNotExpectingOpenTimer"))
{
	TestObject test;
	test.monitor.ReachInAndStartOpenTimer();
	test.exe.AdvanceTime(TimeDuration::Seconds(1));
	REQUIRE(test.exe.RunOne());
	REQUIRE(test.log.PopOneEntry(flags::ERR));
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("StateClosedCanBeStopped"))
{
	TestObject test;
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
}

TEST_CASE(SUITE("StateClosedIgnoresSuspend"))
{
	TestObject test;
	test.monitor.Suspend();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("StartOneBeginsOpening"))
{
	TestObject test;
	test.monitor.StartOne();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
}

TEST_CASE(SUITE("StartOneFailureDoesNotRetry"))
{
	TestObject test;
	test.monitor.StartOne();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("StartingWhileOpeningOneWillRetry"))
{
	TestObject test;
	test.monitor.StartOne();
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
}

TEST_CASE(SUITE("ClosingWhileOpeningOneWillNotRetry"))
{
	TestObject test;
	test.monitor.StartOne();
	test.monitor.Close();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("SuspendWhileOpeningClosingWillNotRetry"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	test.monitor.Suspend();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("LayerCloseAfterStartOneDoesNotRetry"))
{
	TestObject test;
	test.monitor.StartOne();
	test.phys.SignalOpenSuccess();
	REQUIRE((ChannelState::OPEN == test.monitor.GetState()));
	test.phys.BeginClose();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("StopAndCloseDoNothingWhileStopped"))
{
	TestObject test;
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));

	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
	test.monitor.Close();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
}

TEST_CASE(SUITE("StoppedLayerCannotBeStarted"))
{
	TestObject test;
	test.monitor.Shutdown();
	test.monitor.Start();
	test.monitor.StartOne();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
	REQUIRE(test.phys.IsClosed());
}

TEST_CASE(SUITE("ClosedLayerCanBeStarted"))
{
	TestObject test;
	test.monitor.Start();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	REQUIRE(test.phys.IsOpening());
}

TEST_CASE(SUITE("OpeningLayerLogging"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.OnLowerLayerDown();
	REQUIRE(test.log.PopUntil(flags::ERR));
}

TEST_CASE(SUITE("OpeningStartOneGoesToOpeningOne"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.StartOne();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("OpeningStateOpenSuccessGoesToOpenState"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	REQUIRE((ChannelState::OPEN == test.monitor.GetState()));
	REQUIRE(1 ==  test.monitor.mOpenCallbackCount); //check that the callback fired
}

TEST_CASE(SUITE("StopWhileOpeningWaitsForOpenFailure"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Shutdown();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
}

TEST_CASE(SUITE("CloseWhileOpeningWaitsForOpenFailureThenWaits"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
}

TEST_CASE(SUITE("CloseWhileOpeningAndThenStop"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Close();
	test.monitor.Shutdown();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
}

TEST_CASE(SUITE("OpenFailureGoesToWaitingAndExponentialBackoff"))
{
	TestObject test;
	test.monitor.Start();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumPendingTimers());
	REQUIRE(1000 == test.exe.NextTimerExpiration().milliseconds);
	test.exe.AdvanceTime(TimeDuration::Seconds(1));
	REQUIRE(test.exe.RunOne());
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumPendingTimers());
	REQUIRE(3000 ==  test.exe.NextTimerExpiration().milliseconds);
}

TEST_CASE(SUITE("OpenFailureGoesToClosedIfSuspended"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.Suspend();
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
	REQUIRE(0 ==  test.exe.NumActive());
}


TEST_CASE(SUITE("ShutdownCallsInheritedMethod"))
{
	TestObject test;
	REQUIRE(0 ==  test.exe.NumActive());
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
	REQUIRE(0 ==  test.exe.NumActive());
	REQUIRE(1 == test.monitor.mShutdownCallbackCount);
}


TEST_CASE(SUITE("ShutdownWhileWaitingCancelsTimer"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
	REQUIRE(0 ==  test.exe.NumActive());
}

TEST_CASE(SUITE("LayerKeepsTryingToOpen"))
{
	TestObject test;
	test.monitor.Start();

	for(size_t i = 0; i < 3; ++i)
	{
		REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
		test.phys.SignalOpenFailure();
		REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
		test.exe.AdvanceTime(TimeDuration::Seconds(10));
		REQUIRE(test.exe.RunOne());
	}
}

TEST_CASE(SUITE("CloseWhileWaitingDoesNothing"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Close();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumPendingTimers());
}

TEST_CASE(SUITE("LayerCloseWhileOpen"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.phys.BeginClose();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
}

TEST_CASE(SUITE("RequestCloseWhileOpen"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Close();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
}

TEST_CASE(SUITE("RequestStopWhileOpen"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
}

TEST_CASE(SUITE("LayerSuspendWhileOpenDontRetry"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Suspend();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("SuspendWhileWaitingCancelsTimer"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Suspend();
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
	REQUIRE(0 ==  test.exe.NumActive());
}


