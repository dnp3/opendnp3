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

#include <functional>

#include <openpal/util/ToHex.h>
#include <openpal/container/Buffer.h>

#include <opendnp3/Route.h>

#include <dnp3mocks/MockFrameSink.h>

#include "mocks/LinkLayerRouterTest.h"

#include <testlib/BufferHelpers.h>
#include <testlib/HexConversions.h>

using namespace opendnp3;
using namespace openpal;
using namespace testlib;

#define SUITE(name) "LinkLayerRouterSuite - " name

// Test that frames with unknown destinations are correctly logged
TEST_CASE(SUITE("UnknownDestination"))
{
	LinkLayerRouterTest t;

	MockFrameSink mfs;

	Route route(1, 1024);

	REQUIRE(t.router.AddContext(mfs, route));
	REQUIRE(!t.phys.IsOpening());
	REQUIRE(t.router.Enable(&mfs));
	REQUIRE(t.phys.IsOpening());
	t.phys.SignalOpenSuccess();


	t.phys.TriggerRead("05 64 05 C0 01 00 00 04 E9 21");
	REQUIRE(t.log.NextErrorCode() == DLERR_UNKNOWN_ROUTE);
}

// Test that the router rejects sends until it is online
TEST_CASE(SUITE("LayerNotOnline"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	REQUIRE(t.router.AddContext(mfs, route));
	REQUIRE(t.router.Enable(&mfs));
	RSlice buffer;
	t.router.BeginTransmit(buffer, mfs);
	REQUIRE(t.log.PopUntil(flags::ERR));
}

// Test that the router rejects sends until it is online
TEST_CASE(SUITE("AutomaticallyClosesWhenAllContextsAreRemoved"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	t.router.AddContext(mfs, route);
	REQUIRE(t.router.Enable(&mfs));
	REQUIRE((ChannelState::OPENING == t.router.GetState()));
	REQUIRE(t.router.Remove(&mfs));
	REQUIRE((ChannelState::OPENING == t.router.GetState()));
	t.phys.SignalOpenFailure();
	REQUIRE((ChannelState::CLOSED == t.router.GetState()));
}

/// Test that router is correctly clears the send buffer on close
TEST_CASE(SUITE("CloseBehavior"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	t.router.AddContext(mfs, route);
	REQUIRE(t.router.Enable(&mfs));
	t.phys.SignalOpenSuccess();

	ByteStr buffer(292);
	t.router.BeginTransmit(buffer.ToRSlice(), mfs); // puts the router in the send state

	REQUIRE(t.phys.NumWrites() ==  1);
	t.phys.BeginClose(); //we're both reading and writing so this doesn't trigger a callback yet
	REQUIRE(mfs.mLowerOnline);
	t.phys.SignalSendFailure();
	REQUIRE(mfs.mLowerOnline);
	t.phys.SignalReadFailure();

	// now the layer should go offline, this should clear the transmitt queue,
	// the router should also try to restart
	REQUIRE_FALSE(mfs.mLowerOnline);

	REQUIRE(ChannelState::WAITING == t.router.GetState());
	t.exe.AdvanceTime(TimeDuration::Milliseconds(100));
	REQUIRE(t.exe.RunOne());
	REQUIRE(ChannelState::OPENING == t.router.GetState());

	t.phys.ClearBuffer();
	t.phys.SignalOpenSuccess();

	t.router.BeginTransmit(buffer.ToRSlice(), mfs);
	REQUIRE(t.phys.NumWrites() ==  2);
	REQUIRE(t.phys.GetBufferAsHexString() == ToHex(buffer.ToRSlice()));
	t.phys.SignalSendSuccess();
	REQUIRE(t.phys.NumWrites() ==  2);
}

TEST_CASE(SUITE("ReentrantCloseWorks"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	t.router.AddContext(mfs, route);
	t.router.Enable(&mfs);
	t.phys.SignalOpenSuccess();
	REQUIRE(mfs.mLowerOnline);
	mfs.AddAction(std::bind(&asiodnp3::LinkLayerRouter::Shutdown, &t.router));

	Buffer buffer(292);
	auto writeTo = buffer.GetWSlice();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);
	t.phys.TriggerRead(ToHex(frame));

	REQUIRE(t.log.IsLogErrorFree());
}

/// Test that the second bind fails when a non-unique address is added
TEST_CASE(SUITE("MultiAddressBindError"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	REQUIRE(t.router.AddContext(mfs, route));
	REQUIRE_FALSE(t.router.AddContext(mfs, route));
}

/// Test that the second bind fails when a non-unique context is added
TEST_CASE(SUITE("MultiContextBindError"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	REQUIRE(t.router.AddContext(mfs, Route(1, 1024)));
	REQUIRE_FALSE(t.router.AddContext(mfs, Route(1, 2048)));
}

/// Test that router correctly buffers and sends frames from multiple contexts
TEST_CASE(SUITE("MultiContextSend"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs1;
	MockFrameSink mfs2;

	Route route1(1, 1024);
	Route route2(1, 2048);

	t.router.AddContext(mfs1, route1);
	t.router.Enable(&mfs1);
	t.router.AddContext(mfs2, route2);
	t.router.Enable(&mfs2);

	Buffer buffer(292);

	t.phys.SignalOpenSuccess();
	t.router.BeginTransmit(buffer.ToRSlice(), mfs1);
	t.router.BeginTransmit(buffer.ToRSlice(), mfs2);
	REQUIRE(t.phys.NumWrites() ==  1);
	t.phys.SignalSendSuccess();
	REQUIRE(t.phys.NumWrites() ==  2);
	t.phys.SignalSendSuccess();
	REQUIRE(t.phys.NumWrites() ==  2);
}

/// Test that the router correctly clear the receive buffer when the layer closes
TEST_CASE(SUITE("LinkLayerRouterClearsBufferOnLowerLayerDown"))
{
	LinkLayerRouterTest t;
	MockFrameSink mfs;
	Route route(1, 1024);
	t.router.AddContext(mfs, route);
	REQUIRE(t.router.Enable(&mfs));
	t.phys.SignalOpenSuccess();
	t.phys.TriggerRead("05 64 D5 C4 00 04 01 00 F0 BC C0 C0 01 3C 01 06 FF 50");
	REQUIRE(0 ==  mfs.m_num_frames);
	t.phys.SignalReadFailure(); // closes the layer

	t.exe.AdvanceTime(TimeDuration::Milliseconds(100));
	REQUIRE(t.exe.RunMany());

	t.phys.ClearBuffer();
	t.phys.SignalOpenSuccess();

	Buffer buffer(292);
	auto writeTo = buffer.GetWSlice();
	auto frame = LinkFrame::FormatAck(writeTo, true, false, 1024, 1, nullptr);
	t.phys.TriggerRead(ToHex(frame));

	REQUIRE(1 ==  mfs.m_num_frames);
}

