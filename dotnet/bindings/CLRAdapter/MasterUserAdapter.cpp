
#include "Stdafx.h"
#include "MasterUserAdapter.h"


namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			
MasterUserAdapter::MasterUserAdapter(Automatak::DNP3::Interface::IMasterUser^ proxy_) : proxy(proxy_)
{}
				
opendnp3::UpdateKeyMode MasterUserAdapter::GetUpdateKey(openpal::ReadBufferView& key)
{
	auto keyInfo = proxy->DefaultUserUpdateKey;
	
	this->keyBuffer = std::make_unique<openpal::Buffer>(keyInfo->key->Length);
	
	for (int i = 0; i < keyInfo->key->Length; ++i)
	{
		(*keyBuffer)[i] = keyInfo->key[i];
	}

	key = keyBuffer->ToReadOnly();

	return (opendnp3::UpdateKeyMode) keyInfo->keyMode;	
}

		


		}
	}
}


