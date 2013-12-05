#ifndef __TIME_TRANSACTION_H_
#define	__TIME_TRANSACTION_H_

#include "ITransactable.h"
#include "IDataObserver.h"

#include <openpal/UTCTimestamp.h>

namespace opendnp3
{
class TimeTransaction : private Transaction
{
public:
	TimeTransaction(IDataObserver* apObserver, openpal::UTCTimestamp aTimestamp);

	void Update(Binary aMeas, size_t aIndex);
	void Update(Analog aMeas, size_t aIndex);
	void Update(Counter aMeas, size_t aIndex);
	void Update(ControlStatus aMeas, size_t aIndex);
	void Update(SetpointStatus aMeas, size_t aIndex);

private:
	IDataObserver* mpObserver;
	openpal::UTCTimestamp mTimestamp;
};
}

#endif
