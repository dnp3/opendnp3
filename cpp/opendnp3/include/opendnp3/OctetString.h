#ifndef __OCTET_STRING_H_
#define __OCTET_STRING_H_

#include "OctetData.h"

namespace opendnp3
{

class OctetString : public OctetData
{
public:	

	OctetString() : OctetData()
	{}
	
	OctetString(const OctetString& arData) : OctetData(arData)
	{}

	OctetString(const uint8_t* apValue, size_t aSize) : OctetData(apValue, aSize)
	{}

};

}

/* vim: set ts=4 sw=4: */

#endif

