#ifndef __DATABASE_INTERFACES_H_
#define __DATABASE_INTERFACES_H_

#include "DNPDatabaseTypes.h"

#include <openpal/Visibility.h>

namespace opendnp3
{

// @section desc Used by the database
class DLL_LOCAL IEventBuffer
{
public:

	virtual ~IEventBuffer() {}

	virtual void Update(const Binary& arEvent, PointClass aClass, size_t aIndex) = 0;

	virtual void Update(const Analog& arEvent, PointClass aClass, size_t aIndex) = 0;

	virtual void Update(const Counter& arEvent, PointClass aClass, size_t aIndex) = 0;

	virtual void Update(const VtoData& arEvent, PointClass aClass, size_t aIndex) = 0;

	virtual size_t NumVtoEventsAvailable() = 0;

};

}


#endif

