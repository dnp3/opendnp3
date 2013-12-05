#ifndef __CHANGE_BUFFER_H_
#define __CHANGE_BUFFER_H_

#include <opendnp3/DataTypes.h>
#include <opendnp3/IDataObserver.h>
#include <opendnp3/SubjectBase.h>
#include <openpal/Visibility.h>

#include <queue>
#include <mutex>

namespace opendnp3
{

/** Moves measurement data across thread boundaries.
*/
class DLL_LOCAL ChangeBuffer : public IDataObserver, public SubjectBase
{

public:

	ChangeBuffer() : mNotify(false) {}

	void _Start();
	void _End();
	void _Update(const Binary& arPoint, size_t aIndex);
	void _Update(const Analog& arPoint, size_t aIndex);
	void _Update(const Counter& arPoint, size_t aIndex);
	void _Update(const ControlStatus& arPoint, size_t aIndex);
	void _Update(const SetpointStatus& arPoint, size_t aIndex);
	size_t FlushUpdates(IDataObserver* apObserver);

	void Clear();

private:

	template <class T>
	static void Dispatch(IDataObserver* apObs, T& arMeas, size_t aIndex) {
		apObs->Update(arMeas, aIndex);
	}


	void _Clear();

	bool mNotify;

	std::deque<std::function<void (IDataObserver*)>> mChangeQueue;
	std::mutex mMutex;
};

}

#endif

