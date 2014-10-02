

#ifndef __MASTER_APPLICATION_ADAPTER_H_
#define __MASTER_APPLICATION_ADAPTER_H_

using namespace System::Collections::ObjectModel;

#include <vcclr.h>

#include <opendnp3/master/IMasterApplication.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			private class MasterApplicationAdapter : public opendnp3::IMasterApplication
			{
			public:
				MasterApplicationAdapter(Automatak::DNP3::Interface::IMasterApplication^ proxy);

				virtual openpal::UTCTimestamp Now() override final;

				virtual void OnReceiveIIN(const opendnp3::IINField& iin) override final;

				virtual void OnTaskStateChange(opendnp3::TaskId id, opendnp3::TaskState state) override final;

				virtual bool AssignClassDuringStartup() override final { return false; }

				virtual void ConfigureAssignClassRequest(opendnp3::HeaderWriter& writer) override final {}

			private:
				gcroot < Automatak::DNP3::Interface::IMasterApplication^ > proxy;
			};


		}
	}
}

#endif
