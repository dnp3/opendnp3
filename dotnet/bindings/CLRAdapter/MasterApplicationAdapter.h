

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

				virtual void OnStateChange(opendnp3::LinkStatus value) override final;

				virtual openpal::UTCTimestamp Now() override final;

				virtual void OnReceiveIIN(const opendnp3::IINField& iin) override final;

				virtual void OnTaskStart(opendnp3::MasterTaskType type, int userId) override final;

				virtual void OnTaskComplete(opendnp3::MasterTaskType type, opendnp3::TaskCompletion result, int userId) override final;

				virtual bool AssignClassDuringStartup() override final;

				virtual void ConfigureAssignClassRequest(opendnp3::HeaderWriter& writer) override final;

			private:

				static opendnp3::GroupVariationID Convert(PointClass clazz);

				gcroot < Automatak::DNP3::Interface::IMasterApplication^ > proxy;
			};


		}
	}
}

#endif
