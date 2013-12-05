#include <openpal/ToHex.h>

#include <sstream>

namespace openpal
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES

char toHex(char c)
{
	if(c > 9) return 'A' + (c - 10);
	return '0' + c;
}

std::string toHex(const uint8_t* apBuff, size_t aLength, bool spaced)
{
	std::ostringstream oss;
	size_t last = aLength - 1;
	for(size_t i = 0; i < aLength; i++) {
		char c = apBuff[i];
		oss << toHex((c & 0xf0) >> 4) << toHex(c & 0xf);
		if(spaced && i != last)oss << " ";
	}
	return oss.str();
}

std::string toHex(const ReadOnlyBuffer& arBuffer, bool spaced)
{
	return toHex(arBuffer, arBuffer.Size(), spaced);
}


std::string ByteToHex(uint8_t b)
{
	std::ostringstream oss;
	oss << toHex((b & 0xf0) >> 4) << toHex(b & 0xf);
	return oss.str();
}

#endif

}

