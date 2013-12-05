#ifndef __DATABASE_TEST_OBJECT_H_
#define __DATABASE_TEST_OBJECT_H_

#include <queue>
#include <opendnp3/Database.h>
#include <asiopal/Log.h>

namespace opendnp3
{

class MockEventBuffer : public IEventBuffer
{
public:

	virtual ~MockEventBuffer() {}

	void Update(const Binary& arEvent, PointClass aClass, size_t aIndex) {
		BinaryInfo v(arEvent, aClass, aIndex);
		mBinaryEvents.push_back(v);
	}

	void Update(const Analog& arEvent, PointClass aClass, size_t aIndex) {
		AnalogInfo v(arEvent, aClass, aIndex);
		mAnalogEvents.push_back(v);
	}

	void Update(const Counter& arEvent, PointClass aClass, size_t aIndex) {
		CounterInfo v(arEvent, aClass, aIndex);
		mCounterEvents.push_back(v);
	}

	void Update(const VtoData& arEvent, PointClass aClass, size_t aIndex) {
		VtoDataInfo v(arEvent, aClass, aIndex);
		mVtoEvents.push_back(v);
	}

	size_t NumVtoEventsAvailable() {
		return std::numeric_limits<size_t>::max() - mVtoEvents.size();
	}

	std::deque<BinaryInfo> mBinaryEvents;
	std::deque<AnalogInfo> mAnalogEvents;
	std::deque<CounterInfo> mCounterEvents;
	std::deque<VtoDataInfo> mVtoEvents;
};

class DatabaseTestObject
{
public:
	DatabaseTestObject(openpal::LogLevel aLevel = openpal::LogLevel::Info) :
		db(openpal::Logger(&log, aLevel, "test")) {
		db.SetEventBuffer(&buffer);
	}

	asiopal::EventLog log;
	MockEventBuffer buffer;
	Database db;
};

}

#endif

