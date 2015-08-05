
#include "SAConversions.h"

#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			secauth::UserStatusChange SAConversions::Convert(UserStatusChange^ sc)
			{
				auto userPublicKey = Conversions::Convert(sc->userPublicKey);
				auto certificationData = Conversions::Convert(sc->certificationData);
			
				return secauth::UserStatusChange(
					(opendnp3::KeyChangeMethod) sc->keyChangeMethod,
					(opendnp3::UserOperation) sc->userOperation,
					sc->statusChangeSeqNum,
					sc->userRole,
					sc->userRoleExpDays,
					Conversions::ConvertString(sc->userName),
					userPublicKey.ToRSlice(),
					certificationData.ToRSlice()
				);
			}

		}
	}
}

