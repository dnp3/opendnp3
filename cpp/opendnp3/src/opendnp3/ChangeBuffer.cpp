#include "ChangeBuffer.h"

#include <assert.h>

namespace opendnp3
{

size_t ChangeBuffer::FlushUpdates(IDataObserver* apObserver)
{
	assert(this->InProgress());
	size_t count = this->mChangeQueue.size();
	if(count > 0) {
		Transaction t(apObserver);
for(auto f: mChangeQueue) f(apObserver);
	}
	this->Clear();
	return count;
}

void ChangeBuffer::_Start()
{
	mMutex.lock();
}

void ChangeBuffer::_End()
{
	bool notify = mNotify;
	mNotify = false;
	mMutex.unlock();
	if(notify) this->NotifyObservers();
}

void ChangeBuffer::_Update(const Binary& arPoint, size_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Binary>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::_Update(const Analog& arPoint, size_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Analog>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::_Update(const Counter& arPoint, size_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<Counter>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::_Update(const ControlStatus& arPoint, size_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<ControlStatus>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::_Update(const SetpointStatus& arPoint, size_t aIndex)
{
	mChangeQueue.push_back(std::bind(&ChangeBuffer::Dispatch<SetpointStatus>, std::placeholders::_1, arPoint, aIndex));
	mNotify = true;
}

void ChangeBuffer::Clear()
{
	assert(this->InProgress());
	_Clear();
}

void ChangeBuffer::_Clear()
{
	mChangeQueue.clear();
}

}


