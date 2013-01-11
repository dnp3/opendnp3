
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

