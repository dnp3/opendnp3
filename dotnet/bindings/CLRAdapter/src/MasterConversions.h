#ifndef __MASTER_CONVERSIONS_H_
#define __MASTER_CONVERSIONS_H_

using namespace System::Collections::ObjectModel;
using namespace System::Collections::Generic;

#include "CommandCallbackAdapter.h"

#include <vector>

#include <asiodnp3/HeaderTypes.h>
#include <opendnp3/master/TaskConfig.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class MasterConversions
			{
			public:

				static std::vector<asiodnp3::Header> ConvertToVectorOfHeaders(IEnumerable<Header^>^ headers);

				static bool Convert(Header^ header, asiodnp3::Header& output);

				static asiodnp3::Header Convert(Header^ header);
				static asiodnp3::Header ConvertCount8(CountHeader^ header);
				static asiodnp3::Header ConvertCount16(CountHeader^ header);
				static asiodnp3::Header ConvertRange8(RangeHeader^ header);
				static asiodnp3::Header ConvertRange16(RangeHeader^ header);

				static opendnp3::TaskConfig Convert(TaskConfig^ config, ITaskCallback^ wrapper);
				static opendnp3::TaskConfig Convert(TaskConfig^ config);
				static opendnp3::TaskId Convert(TaskId^ taskId);
				static opendnp3::ITaskCallback* CreateTaskCallback(ITaskCallback^ callback);
			
			};

		}
	}
}
#endif
