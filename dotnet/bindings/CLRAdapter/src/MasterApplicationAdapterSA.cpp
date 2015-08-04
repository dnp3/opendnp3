
#include "MasterApplicationAdapterSA.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			
			MasterApplicationAdapterSA::MasterApplicationAdapterSA(Automatak::DNP3::Interface::IMasterApplicationSA^ proxy) :
				MasterApplicationAdapter<secauth::IMasterApplicationSA>(proxy),
				saproxy(proxy)
			{}

			void MasterApplicationAdapterSA::PersistNewUpdateKey(const std::string& username, opendnp3::User user, const secauth::UpdateKey& key)
			{
				saproxy->PersistNewUpdateKey(
					Conversions::ConvertString(username),
					User::FromNumber(user.GetId()),
					MasterConversions::Convert(key)
				);
			}

		}
	}
}


