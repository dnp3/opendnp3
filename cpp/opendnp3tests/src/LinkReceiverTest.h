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
#ifndef __LINK_RECEIVER_TEST_H_
#define __LINK_RECEIVER_TEST_H_

#include <opendnp3/link/LinkLayerReceiver.h>

#include "LogTester.h"
#include "BufferHelpers.h"
#include "MockFrameSink.h"

#include <cstring>
#include <assert.h>

namespace opendnp3
{

class LinkReceiverTest
{
public:
	LinkReceiverTest(openpal::LogLevel aLevel = openpal::LogLevel::Warning, bool aImmediate = false) :
		log(),
		mSink(),
		mRx(openpal::Logger(&log, aLevel, "ReceiverTest"), &mSink)
	{}

	void WriteData(const LinkFrame& arFrame)
	{
		auto buff = mRx.WriteBuff();
		assert(arFrame.GetSize() <= buff.Size());
		memcpy(buff, arFrame.GetBuffer(), arFrame.GetSize());
		mRx.OnRead(arFrame.GetSize());
	}

	void WriteData(const std::string& arHex)
	{
		HexSequence hs(arHex);
		auto buff = mRx.WriteBuff();
		assert(hs.Size() <= buff.Size());
		memcpy(buff, hs, hs.Size());
		mRx.OnRead(hs.Size());
	}

	LogTester log;
	MockFrameSink mSink;
	LinkLayerReceiver mRx;
};

}

#endif

