
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

		}
	}
}


