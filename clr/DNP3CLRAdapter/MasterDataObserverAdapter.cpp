#include "MasterDataObserverAdapter.h"
#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{
MasterDataObserverAdapter::MasterDataObserverAdapter(DNP3::Interface::IDataObserver ^ proxy) : proxy(proxy)
{}

void MasterDataObserverAdapter::Start()
{
	proxy->Start();
}

void MasterDataObserverAdapter::Update(const opendnp3::Binary& arPoint, uint16_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::Update(const opendnp3::Analog& arPoint, uint16_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::Update(const opendnp3::Counter& arPoint, uint16_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::Update(const opendnp3::FrozenCounter& arPoint, uint16_t aIndex)
{
	// TODO
}

void MasterDataObserverAdapter::Update(const opendnp3::ControlStatus& arPoint, uint16_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::Update(const opendnp3::SetpointStatus& arPoint, uint16_t aIndex)
{
	proxy->Update(Conversions::convertMeas(arPoint), aIndex);
}

void MasterDataObserverAdapter::End()
{
	proxy->End();
}

}
}


