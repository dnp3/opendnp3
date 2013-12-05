#ifndef __TRANSPORT_TEST_OBJECT_H_
#define __TRANSPORT_TEST_OBJECT_H_

#include <asiopal/Log.h>
#include <opendnp3/TransportLayer.h>

#include <vector>
#include <string>

#include "MockLowerLayer.h"
#include "MockUpperLayer.h"
#include "LogTester.h"

namespace opendnp3
{

class TransportTestObject
{
public:
	TransportTestObject(bool aOpenOnStart = false, openpal::LogLevel aLevel = openpal::LogLevel::Info, bool aImmediate = false);

	// Generate a complete packet sequence inside the vector and
	// return the corresponding reassembled APDU
	std::string GeneratePacketSequence(std::vector<std::string>&, size_t aNumPackets, size_t aLastPacketLength);

	// Get a Sequence of data w/ optional header
	std::string GetData(const std::string& arHdr, uint8_t aSeed = 0, size_t aLength = TL_MAX_TPDU_PAYLOAD);

	LogTester log;

private:
	openpal::Logger logger;
	TransportLayer transport;

public:

	MockLowerLayer lower;
	MockUpperLayer upper;


};

}

#endif

