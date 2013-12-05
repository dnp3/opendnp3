#include <openpal/ReverseEndian.h>

#include <cstring>

namespace openpal
{

const uint8_t UInt8Simple::Max = std::numeric_limits<uint8_t>::max();
const uint8_t UInt8Simple::Min = std::numeric_limits<uint8_t>::min();

int64_t UInt48LE::Read(const uint8_t* apStart) {

	int64_t  ret = *(apStart);
	ret |= static_cast<int64_t>(*(++apStart)) << 8;
	ret |= static_cast<int64_t>(*(++apStart)) << 16;
	ret |= static_cast<int64_t>(*(++apStart)) << 24;
	ret |= static_cast<int64_t>(*(++apStart)) << 32;
	ret |= static_cast<int64_t>(*(++apStart)) << 40;

	return ret;
}

void UInt48LE::Write(uint8_t* apStart, int64_t aValue) 
{
	if(aValue > MAX) aValue = MAX;

	*(apStart) = static_cast<uint8_t>(aValue & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 8) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 16) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 24) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 32) & 0xFF);
	*(++apStart) = static_cast<uint8_t>((aValue >> 40) & 0xFF);
}

float SingleFloatSafe::Read(const uint8_t* apStart)
{
	return Float<float>::SafeRead(apStart);
}

void SingleFloatSafe::Write(uint8_t* apStart, float aValue)
{
	Float<float>::SafeWrite(apStart, aValue);
}

double DoubleFloatSafe::Read(const uint8_t* apStart)
{
	return Float<double>::SafeRead(apStart);
}

void DoubleFloatSafe::Write(uint8_t* apStart, double aValue)
{
	Float<double>::SafeWrite(apStart, aValue);
}

}
