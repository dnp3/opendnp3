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


#include "AsyncPhysBaseTest.h"
#include "Exception.h"

using namespace opendnp3;
using namespace openpal;

#define SUITE(name) "PhysicalLayerAsyncBaseSuite - " name

TEST_CASE(SUITE("ClosedState"))
{
	AsyncPhysBaseTest t;
	uint8_t buff;
	WriteBuffer wb(&buff, 1);

	t.phys.AsyncClose();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.upper.SendDown("00");
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.AsyncRead(wb);
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalOpenFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalOpenSuccess();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalSendSuccess();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalSendFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));

}

TEST_CASE(SUITE("OpenCloseNotification"))
{
	AsyncPhysBaseTest t;
	const size_t NUM = 3;

	for(size_t i = 1; i <= NUM; ++i)
	{
		t.phys.AsyncOpen();
		t.phys.SignalOpenSuccess();
		REQUIRE(t.upper.GetState().mNumLayerUp ==  i);
		t.phys.AsyncClose();

		t.phys.SignalOpenFailure();
		REQUIRE(t.log.PopOneEntry(flags::ERR));
		t.phys.SignalOpenSuccess();
		REQUIRE(t.log.PopOneEntry(flags::ERR));
		t.phys.SignalSendSuccess();
		REQUIRE(t.log.PopOneEntry(flags::ERR));
		t.phys.SignalSendFailure();
		REQUIRE(t.log.PopOneEntry(flags::ERR));

		REQUIRE(t.phys.NumClose() ==  i);
		REQUIRE(t.upper.GetState().mNumLayerDown ==  i);
	}
}

TEST_CASE(SUITE("ReadState"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();
	t.adapter.StartRead(); //start a read

	t.phys.SignalOpenFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalOpenSuccess();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalSendSuccess();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalSendFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));

	t.phys.TriggerRead("00");
	t.upper.BufferEqualsHex("00");
}

TEST_CASE(SUITE("CloseWhileReading"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();
	t.adapter.StartRead();

	t.phys.AsyncClose();
	REQUIRE(t.phys.NumClose() ==  1);
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0); //layer shouldn't go down until the outstanding read comes back
	t.phys.SignalReadFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  1);
}

TEST_CASE(SUITE("WriteState"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	REQUIRE(t.phys.Size() ==  1);

	t.phys.SignalOpenFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalOpenSuccess();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.TriggerRead("");
	REQUIRE(t.log.PopOneEntry(flags::ERR));
	t.phys.SignalReadFailure();
	REQUIRE(t.log.PopOneEntry(flags::ERR));
}

TEST_CASE(SUITE("CloseWhileWriting"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.phys.AsyncClose();
	REQUIRE(t.phys.NumClose() ==  1);
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0); //layer shouldn't go down until the outstanding write comes back
	t.phys.SignalSendFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  1);
}

TEST_CASE(SUITE("CloseWhileReadingWriting"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.adapter.StartRead();
	t.phys.AsyncClose();
	REQUIRE(t.phys.NumClose() ==  1);
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0);
	t.phys.SignalSendFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0);
	t.phys.SignalReadFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  1);
}

TEST_CASE(SUITE("CloseWhileWritingReading"))
{
	AsyncPhysBaseTest t;
	t.phys.AsyncOpen();
	t.phys.SignalOpenSuccess();

	t.upper.SendDown("00");
	t.adapter.StartRead();
	t.phys.AsyncClose();
	REQUIRE(t.phys.NumClose() ==  1);
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0);
	t.phys.SignalReadFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  0);
	t.phys.SignalSendFailure();
	REQUIRE(t.upper.GetState().mNumLayerDown ==  1);
}

TEST_CASE(SUITE("CloseWhileOpening"))
{
	AsyncPhysBaseTest t;

	t.phys.AsyncOpen();
	t.phys.AsyncClose();
	REQUIRE(t.phys.IsOpening());
	REQUIRE(t.phys.IsClosing());

	/* this could happen for some layers, but we
	   still need to return an open failure to the handler */
	t.phys.SignalOpenSuccess();

	REQUIRE(0 ==  t.upper.GetState().mNumLayerUp);
	REQUIRE(1 ==  t.adapter.GetNumOpenFailure());
}



