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
#include <catch.hpp>

#include <opendnp3/link/PhysicalLayerMonitor.h>
#include <opendnp3/link/PhysicalLayerMonitorStates.h>

#include "MockExecutor.h"
#include "MockPhysicalLayerAsync.h"

#include "Exception.h"
#include "LogTester.h"

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
		mCloseCallbackCount(0)
	{
	}

	void ReachInAndStartOpenTimer()
	{
		this->StartOpenTimer();
	}

	size_t mOpenCallbackCount;
	size_t mCloseCallbackCount;

protected:

	void OnPhysicalLayerOpenSuccessCallback()
	{
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerOpenFailureCallback()
	{
		++mOpenCallbackCount;
	}
	void OnPhysicalLayerCloseCallback()
	{
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

	LogTester log;
	MockExecutor exe;
	MockPhysicalLayerAsync phys;
	ConcretePhysicalLayerMonitor monitor;
};

#define SUITE(name) "PhysicalLayerMonitorTestSuite - " name

TEST_CASE(SUITE("StateClosedExceptions"))
{
	TestObject test;
	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));

	test.monitor.OnLowerLayerUp();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.monitor.OnLowerLayerDown();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
	test.monitor.OnOpenFailure();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));


	REQUIRE((ChannelState::CLOSED == test.monitor.GetState()));
}

TEST_CASE(SUITE("ThrowsIfEverNotExpectingOpenTimer"))
{
	TestObject test;
	test.monitor.ReachInAndStartOpenTimer();
	REQUIRE(test.exe.DispatchOne());
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
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
	test.phys.AsyncClose();
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

TEST_CASE(SUITE("OpeningLayerExceptions"))
{
	TestObject test;
	test.monitor.Start();
	test.monitor.OnLowerLayerDown();
	REQUIRE(test.log.PopOneEntry(LogLevel::Error));
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
	REQUIRE(1 ==  test.exe.NumActive());
	REQUIRE(1000 ==  test.exe.NextTimerExpiration().milliseconds);
	REQUIRE(test.exe.DispatchOne());
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
	test.phys.SignalOpenFailure();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumActive());
	REQUIRE(2000 ==  test.exe.NextTimerExpiration().milliseconds);
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

TEST_CASE(SUITE("ShutdownPostsToTimer"))
{
	TestObject test;
	REQUIRE(0 ==  test.exe.NumActive());
	REQUIRE_FALSE(test.monitor.WaitForShutdown(TimeDuration::Seconds(0)));
	test.monitor.Shutdown();
	REQUIRE((ChannelState::SHUTDOWN == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumActive());
	REQUIRE_FALSE(test.monitor.WaitForShutdown(TimeDuration::Seconds(0)));
	REQUIRE(test.exe.DispatchOne());
	REQUIRE(test.monitor.WaitForShutdown()); //wait indefinitely, but it's already shutdown
}

TEST_CASE(SUITE("ShutdownWhileWaitingCancelsTimer"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Shutdown();
	REQUIRE(test.exe.DispatchOne()); //disptach the shutdown post
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
		REQUIRE(test.exe.DispatchOne());
	}
}

TEST_CASE(SUITE("CloseWhileWaitingDoesNothing"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenFailure();
	test.monitor.Close();
	REQUIRE((ChannelState::WAITING == test.monitor.GetState()));
	REQUIRE(1 ==  test.exe.NumActive());
}

TEST_CASE(SUITE("LayerCloseWhileOpen"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.phys.AsyncClose();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
}

TEST_CASE(SUITE("RequestCloseWhileOpen"))
{
	TestObject test;
	test.monitor.Start();
	test.phys.SignalOpenSuccess();
	test.monitor.Close();
	REQUIRE((ChannelState::OPENING == test.monitor.GetState()));
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


