#ifndef __MASTER_DATA_OBSERVER_H_
#define __MASTER_DATA_OBSERVER_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/IDataObserver.h>
#include <vcclr.h>

using namespace DNP3::Interface;

namespace DNP3
{
namespace Adapter
{
private class MasterDataObserverAdapter : public opendnp3::IDataObserver
{
public:

	MasterDataObserverAdapter(DNP3::Interface::IDataObserver ^ proxy);

	void Update(const opendnp3::Binary& arPoint, uint16_t aIndex);
	void Update(const opendnp3::Analog& arPoint, uint16_t aIndex);
	void Update(const opendnp3::Counter& arPoint, uint16_t aIndex);
	void Update(const opendnp3::FrozenCounter& arPoint, uint16_t aIndex);
	void Update(const opendnp3::ControlStatus& arPoint, uint16_t aIndex);
	void Update(const opendnp3::SetpointStatus& arPoint, uint16_t aIndex);

private:

	gcroot < DNP3::Interface::IDataObserver ^ > proxy;

protected:

	void Start();	
	void End();
};

private ref class MasterDataObserverWrapper
{
public:

	MasterDataObserverWrapper(DNP3::Interface::IDataObserver ^ proxy) :
		mpAdapter(new MasterDataObserverAdapter(proxy))
	{}

	~MasterDataObserverWrapper() {
		delete mpAdapter;
	}

	opendnp3::IDataObserver* Get() {
		return mpAdapter;
	}

private:
	MasterDataObserverAdapter* mpAdapter;
};

}
}

#endif
