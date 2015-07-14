
#include "Stdafx.h"
#include "MasterUserDatabaseAdapter.h"


namespace Automatak
{
namespace DNP3
{
namespace Adapter
{
			
MasterUserDatabaseAdapter::MasterUserDatabaseAdapter(Automatak::DNP3::Interface::IMasterUser^ proxy_) : 
	proxy(proxy_),
	keyBuffer(std::make_unique<openpal::Buffer>(proxy_->UpdateKey->key->Length))
{
	// TODO - get rid of this dirty dirty hack
	auto keyInfo = proxy->UpdateKey;
	for (int i = 0; i < keyInfo->key->Length; ++i)
	{
		(*keyBuffer)[i] = keyInfo->key[i];
	}
}
				
void MasterUserDatabaseAdapter::EnumerateUsers(const std::function<void(const opendnp3::User)>& fun) const
{	
	fun(opendnp3::User(proxy->UserNumber->Number));
}

bool MasterUserDatabaseAdapter::GetUpdateKey(const opendnp3::User& user, opendnp3::UpdateKeyMode& type, openpal::ReadBufferView& key) const
{
	if (user.GetId() != proxy->UserNumber->Number)
	{
		return false;
	}	

	type = (opendnp3::UpdateKeyMode) proxy->UpdateKey->keyMode;
	key = keyBuffer->ToReadOnly();

	return true;
}

bool MasterUserDatabaseAdapter::UserExists(const opendnp3::User& user) const
{
	return user.GetId() == proxy->UserNumber->Number;
}
	
}}} //end ns



