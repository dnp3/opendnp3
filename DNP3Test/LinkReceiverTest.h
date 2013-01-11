
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
// you under the GNU Affero General Public License Version 3.0 
// (the "Additional License"). You may not use these modifications except in 
// compliance with the additional license. You may obtain a copy of the Additional 
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __LINK_RECEIVER_TEST_H_
#define __LINK_RECEIVER_TEST_H_

#include <DNP3/LinkLayerReceiver.h>
#include <APLTestTools/LogTester.h>
#include <APLTestTools/BufferHelpers.h>
#include "MockFrameSink.h"

namespace apl
{
namespace dnp
{

class LinkReceiverTest : public LogTester
{
public:
	LinkReceiverTest(FilterLevel aLevel = LEV_WARNING, bool aImmediate = false) :
		LogTester(aImmediate),
		mSink(),
		mRx(mLog.GetLogger(aLevel, "ReceiverTest"), &mSink)
	{}

	void WriteData(const LinkFrame& arFrame) {
		assert(arFrame.GetSize() <= mRx.NumWriteBytes());
		memcpy(mRx.WriteBuff(), arFrame.GetBuffer(), arFrame.GetSize());
		mRx.OnRead(arFrame.GetSize());
	}

	void WriteData(const std::string& arHex) {
		HexSequence hs(arHex);
		assert(hs.Size() <= mRx.NumWriteBytes());
		memcpy(mRx.WriteBuff(), hs, hs.Size());
		mRx.OnRead(hs.Size());
	}

	MockFrameSink mSink;
	LinkLayerReceiver mRx;
};

}
}

#endif

