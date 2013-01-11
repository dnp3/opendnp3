
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

#include <boost/asio.hpp>
#include <boost/test/unit_test.hpp>
#include <APLTestTools/TestHelpers.h>

#include <APLTestTools/AsyncTestObjectASIO.h>
#include <APLTestTools/LogTester.h>
#include <APL/LowerLayerToPhysAdapter.h>
#include <APLTestTools/MockUpperLayer.h>

#include <Terminal/PhysicalLayerSyncProxy.h>

#include <queue>
#include <mutex>
#include <condition_variable>

using namespace apl;
using namespace boost;

class SyncProxyTestObject : public LogTester, public AsyncTestObjectASIO, public PhysicalLayerSyncProxy
{
public:
	SyncProxyTestObject(FilterLevel aLevel, bool aImmediate = false) :
		LogTester(aImmediate),
		AsyncTestObjectASIO(),
		PhysicalLayerSyncProxy(mLog.GetLogger(aLevel, "SyncProxy"), this->GetService()),
		adapter(mLog.GetLogger(aLevel, "Adapter"), this, false),
		upper(mLog.GetLogger(aLevel, "Upper")) {
		adapter.SetUpperLayer(&upper);
	}

	std::string Read() {
		std::unique_lock<std::mutex> lock(mQueueMutex);
		while(mReadQueue.size() == 0) mQueueCondition.wait(lock);
		std::string ret = mReadQueue.front();
		mReadQueue.pop_front();
		return ret;
	}

	void Write(const std::string&) {

	}

	void Push(const std::string& s) {
		std::unique_lock<std::mutex> lock(mQueueMutex);
		mReadQueue.push_back(s);
		mQueueCondition.notify_all();
	}

	apl::LowerLayerToPhysAdapter adapter;
	apl::MockUpperLayer upper;

private:

	std::mutex mQueueMutex;
	std::condition_variable mQueueCondition;
	std::deque<std::string> mReadQueue;
};

BOOST_AUTO_TEST_SUITE(PhysicalLayerSyncProxy)


BOOST_AUTO_TEST_CASE(Init)
{
	SyncProxyTestObject t(LEV_INFO);
}

BOOST_AUTO_TEST_CASE(OpenReadWrite)
{
	SyncProxyTestObject t(LEV_INFO);
	MockUpperLayer::State s;
	BOOST_REQUIRE(t.upper.StateEquals(s));
	t.AsyncOpen(); ++s.mNumLayerUp;
	t.ProceedUntil(std::bind(&MockUpperLayer::StateEquals, &t.upper, s));

	t.adapter.StartRead();
	t.Push("foo");
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::BufferEqualsString, &t.upper, "foo")));

	t.Push("bar");
	t.adapter.StartRead();
	BOOST_REQUIRE(t.ProceedUntil(std::bind(&MockUpperLayer::BufferEqualsString, &t.upper, "foobar")));

	t.upper.SendDown("00"); ++s.mSuccessCnt;
	t.ProceedUntil(std::bind(&MockUpperLayer::StateEquals, &t.upper, s));

	t.AsyncClose(); ++s.mNumLayerDown;
	t.ProceedUntil(std::bind(&MockUpperLayer::StateEquals, &t.upper, s));
}



BOOST_AUTO_TEST_SUITE_END()

