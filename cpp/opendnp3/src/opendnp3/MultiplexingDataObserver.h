#ifndef __MULTIPLEXING_DATA_OBSERVER_H_
#define __MULTIPLEXING_DATA_OBSERVER_H_

#include <opendnp3/IDataObserver.h>
#include <openpal/Visibility.h>

#include <vector>
#include <mutex>

namespace opendnp3
{

/** DataObserver for sending updates to multiple data observers.
*/
class DLL_LOCAL MultiplexingDataObserver : public IDataObserver
{
public:

	MultiplexingDataObserver();
	MultiplexingDataObserver(IDataObserver* apObserver1);
	MultiplexingDataObserver(IDataObserver* apObserver1, IDataObserver* apObserver2);

	void AddObserver(IDataObserver* apObserver1);

private:

	std::vector<IDataObserver*> mObservers;

	std::mutex mMutex;

	void _Start();

	void _End();

	void _Update(const Binary& arPoint, size_t aIndex);
	void _Update(const Analog& arPoint, size_t aIndex);
	void _Update(const Counter& arPoint, size_t aIndex);
	void _Update(const ControlStatus& arPoint, size_t aIndex);
	void _Update(const SetpointStatus& arPoint, size_t aIndex);

};

}

#endif

