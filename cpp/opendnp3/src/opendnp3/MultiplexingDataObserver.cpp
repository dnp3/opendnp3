#include "MultiplexingDataObserver.h"

namespace opendnp3
{

MultiplexingDataObserver :: MultiplexingDataObserver()
{}

MultiplexingDataObserver :: MultiplexingDataObserver(IDataObserver* apObserver1)
{
	AddObserver(apObserver1);
}

MultiplexingDataObserver :: MultiplexingDataObserver(IDataObserver* apObserver1, IDataObserver* apObserver2)
{
	AddObserver(apObserver1);
	AddObserver(apObserver2);
}

void MultiplexingDataObserver :: AddObserver(IDataObserver* apObserver)
{
	mObservers.push_back(apObserver);
}

void MultiplexingDataObserver :: _Start()
{
	mMutex.lock();
for(auto pObs: mObservers) pObs->Start();
}

void MultiplexingDataObserver :: _End()
{
for(auto pObs: mObservers) pObs->End();
	mMutex.unlock();
}

void MultiplexingDataObserver :: _Update(const Binary& arPoint, size_t aIndex)
{
for(auto pObs: mObservers) pObs->Update(arPoint, aIndex);
}
void MultiplexingDataObserver :: _Update(const Analog& arPoint, size_t aIndex)
{
for(auto pObs: mObservers) pObs->Update(arPoint, aIndex);
}
void MultiplexingDataObserver :: _Update(const Counter& arPoint, size_t aIndex)
{
for(auto pObs: mObservers) pObs->Update(arPoint, aIndex);
}
void MultiplexingDataObserver :: _Update(const ControlStatus& arPoint, size_t aIndex)
{
for(auto pObs: mObservers) pObs->Update(arPoint, aIndex);
}
void MultiplexingDataObserver :: _Update(const SetpointStatus& arPoint, size_t aIndex)
{
for(auto pObs: mObservers) pObs->Update(arPoint, aIndex);
}


}
