#include <opendnp3/TimeTransaction.h>

using namespace openpal;

namespace opendnp3
{
TimeTransaction::TimeTransaction(IDataObserver* apObserver, UTCTimestamp aTimestamp) :
	Transaction(apObserver),
	mpObserver(apObserver),
	mTimestamp(aTimestamp)
{

}

void TimeTransaction::Update(Binary aMeas, size_t aIndex)
{
	aMeas.SetTime(mTimestamp.msSinceEpoch);
	mpObserver->Update(aMeas, aIndex);
}

void TimeTransaction::Update(Analog aMeas, size_t aIndex)
{
	aMeas.SetTime(mTimestamp.msSinceEpoch);
	mpObserver->Update(aMeas, aIndex);
}

void TimeTransaction::Update(Counter aMeas, size_t aIndex)
{
	aMeas.SetTime(mTimestamp.msSinceEpoch);
	mpObserver->Update(aMeas, aIndex);
}

void TimeTransaction::Update(ControlStatus aMeas, size_t aIndex)
{
	aMeas.SetTime(mTimestamp.msSinceEpoch);
	mpObserver->Update(aMeas, aIndex);
}

void TimeTransaction::Update(SetpointStatus aMeas, size_t aIndex)
{
	aMeas.SetTime(mTimestamp.msSinceEpoch);
	mpObserver->Update(aMeas, aIndex);
}

}
