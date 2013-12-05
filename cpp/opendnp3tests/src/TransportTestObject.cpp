#include "TransportTestObject.h"

#include "BufferHelpers.h"

#include <openpal/ToHex.h>

#include <memory>
#include <sstream>

using namespace std;
using namespace openpal;

namespace opendnp3
{

TransportTestObject::TransportTestObject(bool aOpenOnStart, LogLevel aLevel, bool aImmediate) :
	log(),
	logger(Logger(&log, aLevel, "TransportTestObject")),
	transport(logger),
	lower(logger),
	upper(logger)
{
	lower.SetUpperLayer(&transport);
	transport.SetUpperLayer(&upper);

	if(aOpenOnStart) lower.ThisLayerUp();
}

std::string TransportTestObject::GetData(const std::string& arHdr, uint8_t aSeed, size_t aLength)
{
	ByteStr buff(aLength);
	uint8_t val = aSeed;
	for(size_t i = 0; i < aLength; ++i) {
		buff[i] = val;
		++val;
	}

	ostringstream oss;
	if(arHdr.size() > 0) oss << arHdr << " ";
	oss << toHex(buff, buff.Size(), true);
	return oss.str();
}

std::string TransportTestObject::GeneratePacketSequence(vector< std::string >& arVec, size_t aNumPackets, size_t aLastPacketLength)
{
	ostringstream oss;
	for(size_t i = 0; i < aNumPackets; ++i) {
		bool fir = i == 0;
		bool fin = i == (aNumPackets - 1);
		int seq = static_cast<int>(i % 64);
		size_t len = fin ? aLastPacketLength : TL_MAX_TPDU_PAYLOAD;
		uint8_t hdr = TransportTx::GetHeader(fir, fin, seq);
		std::string data = this->GetData("", 0, len); //raw data with no header
		oss << ((i == 0) ? "" : " ") << data; //cache the data in the string stream
		arVec.push_back(toHex(&hdr, 1, true) + " " + data);
	}

	return oss.str();
}

}

