#ifndef __FANOUT_DATA_OBSERVER_H_
#define __FANOUT_DATA_OBSERVER_H_

#include <opendnp3/ChangeBuffer.h>

namespace opendnp3
{

class FanoutDataObserver : public IDataObserver
{
public:

	void AddObserver(IDataObserver* apObserver) {
		mObservers.push_back(apObserver);
	}

	void _Start() {
for(IDataObserver * pObs: mObservers) pObs->Start();
	}
	void _End() {
for(IDataObserver * pObs: mObservers) pObs->End();


	}

	void _Update(const Binary& arPoint, size_t aIndex) {
for(IDataObserver * pObs: mObservers) pObs->Update(arPoint, aIndex);
	}
	void _Update(const Analog& arPoint, size_t aIndex) {
for(IDataObserver * pObs: mObservers) pObs->Update(arPoint, aIndex);
	}
	void _Update(const Counter& arPoint, size_t aIndex) {
for(IDataObserver * pObs: mObservers) pObs->Update(arPoint, aIndex);
	}
	void _Update(const ControlStatus& arPoint, size_t aIndex) {
for(IDataObserver * pObs: mObservers) pObs->Update(arPoint, aIndex);
	}
	void _Update(const SetpointStatus& arPoint, size_t aIndex) {
for(IDataObserver * pObs: mObservers) pObs->Update(arPoint, aIndex);
	}

private:
	std::vector<IDataObserver*> mObservers;
};

}

#endif

