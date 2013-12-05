#ifndef __OPENPAL_REVERSE_ENDIAN_H_
#define __OPENPAL_REVERSE_ENDIAN_H_

#include <cstdint>

#include "SerializationTemplates.h"

namespace openpal {

class UInt8Simple
{
public:

	inline static uint8_t Read(const uint8_t* apStart) 
	{
		return (*apStart);
	}

	inline static void Write(uint8_t* apStart, uint8_t aValue) 
	{
		*(apStart) = aValue;
	}

	const static size_t Size = 1;
	const static uint8_t Max;
	const static uint8_t Min;

	typedef uint8_t Type;
};

class UInt48LE
{
public:

	static int64_t Read(const uint8_t* apStart);	
	static void Write(uint8_t* apStart, int64_t aValue);

	const static int64_t MAX = 281474976710655ULL; // 2^48 -1
	const static size_t Size = 6;
	typedef int64_t Type;
};

class SingleFloatSafe : public Float<float>
{
public:
	static float Read(const uint8_t* apStart);
	static void Write(uint8_t* apStart, float aValue);
};

class DoubleFloatSafe : public Float<double>
{
public:
	static double Read(const uint8_t* apStart);
	static void Write(uint8_t* apStart, double aValue);
};

}

#endif
