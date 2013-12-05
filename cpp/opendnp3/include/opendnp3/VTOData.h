#ifndef __VTO_DATA_H_
#define __VTO_DATA_H_

#include "OctetData.h"

namespace opendnp3
{

class VtoData : public OctetData
{
public:	

	VtoData() : OctetData()
	{}
	
	VtoData(const VtoData& arData) : OctetData(arData)
	{}

	VtoData(const uint8_t* apValue, size_t aSize) : OctetData(apValue, aSize)
	{}

};

}

/* vim: set ts=4 sw=4: */

#endif

