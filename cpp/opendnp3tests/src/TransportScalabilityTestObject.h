#ifndef __TRANSPORT_SCALABILITY_TEST_OBJECT_H_
#define __TRANSPORT_SCALABILITY_TEST_OBJECT_H_

#include "TransportStackPair.h"
#include "AsyncTestObjectASIO.h"
#include "LogTester.h"

#include <asiopal/ASIOExecutor.h>

namespace opendnp3
{

class TransportScalabilityTestObject : public AsyncTestObjectASIO
{
public:
	TransportScalabilityTestObject(
	        LinkConfig aClientCfg,
	        LinkConfig aServerCfg,
	        boost::uint16_t aPortStart,
	        boost::uint16_t aNumPair,
	        openpal::LogLevel aLevel = openpal::LogLevel::Info,
	        bool aImmediate = false);

	~TransportScalabilityTestObject();

	void Start();


	// Test helpers
	bool AllLayersUp();
	bool AllLayerReceived(size_t aNumBytes);
	bool AllLayerEqual(const openpal::ReadOnlyBuffer& arBuffer);

	void SendToAll(const openpal::ReadOnlyBuffer& arBuffer);

	LogTester log;
	std::vector<TransportStackPair*> mPairs;
};

}

#endif
