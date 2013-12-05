#ifndef __TO_HEX_H_
#define __TO_HEX_H_

#include <cstdint>
#include <string>

#include "BufferWrapper.h"

namespace openpal
{

#ifndef OPENDNP3_STRIP_LOG_MESSAGES

char toHex(char c);
std::string ByteToHex(uint8_t b);


std::string toHex(const uint8_t* apBuff, size_t aLength, bool spaced = false);

std::string toHex(const ReadOnlyBuffer& arBuffer, bool spaced = false);

/*
template<class T>
std::string TypeToHex(T x)
{
	return toHex(reinterpret_cast<uint8_t*>(&x), sizeof(T), true);
}
*/

#endif

}

#endif
