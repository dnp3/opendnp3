#include "ObjectHeader.h"

#include <stddef.h>

namespace opendnp3
{

AllObjectsHeader AllObjectsHeader::mInstance;

void IObjectHeader::Get(const uint8_t* apStart, ObjectHeaderField& arData) const
{
	arData.Group = *(apStart);
	arData.Variation = *(++apStart);
	arData.Qualifier = QualifierCodeFromType(*(++apStart));
}

void IObjectHeader::Set(uint8_t* apStart, uint8_t aGrp, uint8_t aVar, QualifierCode aQual) const
{
	*(apStart) = aGrp;
	*(++apStart) = aVar;
	*(++apStart) = QualifierCodeToType(aQual);
}

}

