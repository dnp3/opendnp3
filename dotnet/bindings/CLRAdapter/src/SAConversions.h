#ifndef __SA_CONVERSIONS_H_
#define __SA_CONVERSIONS_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include <secauth/master/UserStatusChange.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class SAConversions
			{
			public:
				
				static secauth::UserStatusChange Convert(UserStatusChange^ statusChange);

							
			};

		}
	}
}
#endif
