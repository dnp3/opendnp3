#ifndef __OCTET_DATA_H_
#define __OCTET_DATA_H_

#include <string>
#include <cstdint>

namespace opendnp3
{

class OctetData
{
public:

	const static size_t MAX_SIZE = 255;

	OctetData();	
	OctetData(const uint8_t* apValue, size_t aSize);
	OctetData(const OctetData&);
	OctetData& operator=( const OctetData& rhs );
	~OctetData();

	const uint8_t* Data() const;

	std::string AsString() const;

	size_t GetSize() const;	

private:	

	uint8_t* mpData;
	size_t mSize;	
};

}

/* vim: set ts=4 sw=4: */

#endif

