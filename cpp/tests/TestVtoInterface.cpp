
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

#include "BufferHelpers.h"
#include "TestHelpers.h"

#include <openpal/Exception.h>

#include <opendnp3/IDataObserver.h>
#include <opendnp3/DataTypes.h>
#include <opendnp3/Log.h>
#include <opendnp3/LogToStdio.h>
#include <opendnp3/DNPConstants.h>
#include <opendnp3/VtoData.h>
#include <opendnp3/VtoDataInterface.h>
#include <opendnp3/VtoReader.h>
#include <opendnp3/VtoWriter.h>

using namespace std;
using namespace opendnp3;
using namespace openpal;

#define MAX_SIZE					(255)

#define MACRO_BZERO(b,len) (memset((b), '\0', (len)), (void) 0)

class VtoCallbackTest : public IVtoCallbacks
{
public:
	VtoCallbackTest(uint8_t aChannelId) : IVtoCallbacks(aChannelId) {
		this->Reset();
	}

	void OnVtoDataReceived(const VtoData& arData);

	void OnVtoRemoteConnectedChanged(bool aIsRemoteOpen);

	void OnDnpConnectedChanged(bool aIsRemoteOpen) {}

	void OnBufferAvailable();

	void Reset() {
		this->numOnVtoDataReceived = 0;
		this->numOnBufferAvailable = 0;

		this->lastOnVtoDataReceived = 0;

		MACRO_BZERO(this->received, 1024);
		this->size = 0;

		this->numRemoteConnectedClosed = 0;
		this->numRemoteConnectedOpened = 0;
		this->lastRemoteConnectionState = false;
	}

	size_t numOnVtoDataReceived;
	size_t numOnBufferAvailable;

	size_t lastOnVtoDataReceived;

	size_t numRemoteConnectedClosed;
	size_t numRemoteConnectedOpened;
	bool lastRemoteConnectionState;

	uint8_t received[4096];
	size_t size;
};

void VtoCallbackTest::OnVtoDataReceived(const VtoData& arData)
{
	size_t aLength = arData.GetSize();
	assert(this->size + aLength <= sizeof(received));
	memcpy(&this->received[this->size], arData.mpData, aLength);
	this->size += aLength;

	this->lastOnVtoDataReceived = aLength;
	++this->numOnVtoDataReceived;
}

void VtoCallbackTest::OnVtoRemoteConnectedChanged(bool aIsRemoteOpen)
{
	if(aIsRemoteOpen) ++this->numRemoteConnectedOpened;
	else ++this->numRemoteConnectedClosed;

	this->lastRemoteConnectionState = aIsRemoteOpen;
}

void VtoCallbackTest::OnBufferAvailable()
{
	++this->numOnBufferAvailable;
}

BOOST_AUTO_TEST_SUITE(VtoInterfaceTests)
BOOST_AUTO_TEST_CASE(VtoWriteOverflow)
{
	EventLog log;
	const size_t numChunks = 3;
	const size_t emptySize = numChunks * MAX_SIZE;
	VtoWriter writer(Logger(&log, LEV_DEBUG, "writer"), numChunks);

	const size_t dataSize = MAX_SIZE * 10;
	uint8_t data[dataSize];

	size_t len;

	for (size_t i = 0; i < dataSize; ++i)
		data[i] = static_cast<uint8_t>(i % 255);

	/* Check the default size to make sure we have full space available */
	BOOST_REQUIRE_EQUAL(writer.NumBytesAvailable(), emptySize);

	/* Write all data, only some should get written */
	len = writer.Write(data, dataSize, 1);
	BOOST_REQUIRE_EQUAL(len, emptySize);
	BOOST_REQUIRE(len != dataSize);
	BOOST_REQUIRE_EQUAL(writer.NumBytesAvailable(), 0);
}

BOOST_AUTO_TEST_CASE(VtoReaderRegisterChannels)
{
	EventLog log;
	VtoReader reader(Logger(&log, LEV_WARNING, "test"));

	VtoCallbackTest channel(1);

	/* Register a new channel */
	BOOST_REQUIRE_NO_THROW(reader.AddVtoChannel(&channel));

	/* Register a duplicate channel */
	BOOST_REQUIRE_THROW(reader.AddVtoChannel(&channel), ArgumentException);

	/* Unregister a channel*/
	BOOST_REQUIRE_NO_THROW(reader.RemoveVtoChannel(&channel));

	/* Unregister a bad channel */
	BOOST_REQUIRE_THROW(reader.RemoveVtoChannel(&channel), ArgumentException);
}

// TODO - Factor this test into smaller subtests, way too much going on here
BOOST_AUTO_TEST_CASE(VtoReaderUpdate)
{
	EventLog log;

	VtoReader reader(Logger(&log, LEV_WARNING, "test"));

	const size_t size = 6;
	uint8_t buffer[size];

	VtoData data;

	VtoCallbackTest channel1(1);
	VtoCallbackTest channel2(2);

	/*
	 * Create a data sequence of "abcdef" that will be used for all of the
	 * tests below.
	 */
	for (size_t i = 0; i < size; i++) {
		buffer[i] = static_cast<uint8_t>('a' + i);
	}

	data.Copy(buffer, size);

	/* Register a new channel */
	BOOST_REQUIRE_NO_THROW(reader.AddVtoChannel(&channel1));
	BOOST_REQUIRE_NO_THROW(reader.AddVtoChannel(&channel2));

	/* Check that data for an unregistered channel is ignored */
	channel1.Reset();
	channel2.Reset();
	{
		Transaction tr(reader);
		reader.Update(data, 3);
	}

	BOOST_REQUIRE_EQUAL(channel1.numOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel1.lastOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel1.size, 0);

	BOOST_REQUIRE_EQUAL(channel2.numOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel2.lastOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel2.size, 0);

	/* Check that data for a registered channel is stored */
	channel1.Reset();
	channel2.Reset();
	{
		Transaction tr(reader);
		reader.Update(data, 1);
	}

	BOOST_REQUIRE_EQUAL(channel1.numOnVtoDataReceived, 1);
	BOOST_REQUIRE_EQUAL(channel1.lastOnVtoDataReceived, size);
	BOOST_REQUIRE_EQUAL(channel1.size, size);

	BOOST_REQUIRE_EQUAL(channel2.numOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel2.lastOnVtoDataReceived, 0);
	BOOST_REQUIRE_EQUAL(channel2.size, 0);

	/* Check a sequence of data stores */
	{
		channel1.Reset();
		channel2.Reset();

		/* Check that data for a registered channel is stored */
		{
			Transaction tr(reader);
			reader.Update(data, 1);
		}

		BOOST_REQUIRE_EQUAL(channel1.numOnVtoDataReceived, 1);
		BOOST_REQUIRE_EQUAL(channel1.lastOnVtoDataReceived, size);
		BOOST_REQUIRE_EQUAL(channel1.size, size);

		BOOST_REQUIRE_EQUAL(channel2.numOnVtoDataReceived, 0);
		BOOST_REQUIRE_EQUAL(channel2.lastOnVtoDataReceived, 0);
		BOOST_REQUIRE_EQUAL(channel2.size, 0);

		/* Check that data for different channels are stored */
		data.Copy(buffer, size / 2);
		{
			Transaction tr(reader);
			reader.Update(data, 2);
		}

		BOOST_REQUIRE_EQUAL(channel1.numOnVtoDataReceived, 1);
		BOOST_REQUIRE_EQUAL(channel1.lastOnVtoDataReceived, size);
		BOOST_REQUIRE_EQUAL(channel1.size, size);

		BOOST_REQUIRE_EQUAL(channel2.numOnVtoDataReceived, 1);
		BOOST_REQUIRE_EQUAL(channel2.lastOnVtoDataReceived, size / 2);
		BOOST_REQUIRE_EQUAL(channel2.size, size / 2);

		/* Check that multiple data for a channel is stored */
		data.Copy(buffer, size / 2);
		{
			Transaction tr(reader);
			reader.Update(data, 1);
		}

		BOOST_REQUIRE_EQUAL(channel1.numOnVtoDataReceived, 2);
		BOOST_REQUIRE_EQUAL(channel1.lastOnVtoDataReceived, size / 2);
		BOOST_REQUIRE_EQUAL(channel1.size, size + (size / 2));

		BOOST_REQUIRE_EQUAL(channel2.numOnVtoDataReceived, 1);
		BOOST_REQUIRE_EQUAL(channel2.lastOnVtoDataReceived, size / 2);
		BOOST_REQUIRE_EQUAL(channel2.size, size / 2);

		/* Make sure the final received buffer stream looks proper */
		char ch1Data[] = "abcdefabc";
		for (size_t i = 0; i < channel1.size; ++i) {
			BOOST_REQUIRE_EQUAL(channel1.received[i], ch1Data[i]);
		}

		char ch2Data[] = "abc";
		for (size_t i = 0; i < channel2.size; ++i) {
			BOOST_REQUIRE_EQUAL(channel2.received[i], ch2Data[i]);
		}
	}
}
BOOST_AUTO_TEST_SUITE_END()

/* vim: set ts=4 sw=4: */
