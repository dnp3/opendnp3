
#include "Stdafx.h"
#include "MasterApplicationAdapter.h"

#include <opendnp3/objects/Group60.h>
#include <openpal/serialization/Serialization.h>

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{


			MasterApplicationAdapter::MasterApplicationAdapter(Automatak::DNP3::Interface::IMasterApplication^ proxy_) : proxy(proxy_)
			{}

			openpal::UTCTimestamp MasterApplicationAdapter::Now()
			{
				auto milliseconds = proxy->GetMillisecondsSinceEpoch();
				return openpal::UTCTimestamp(milliseconds);
			}

			void MasterApplicationAdapter::OnReceiveIIN(const opendnp3::IINField& iin)
			{
				IINField ^iinField = gcnew IINField((Automatak::DNP3::Interface::LSBMask)iin.LSB, (Automatak::DNP3::Interface::MSBMask)iin.MSB);
				proxy->OnReceiveIIN(iinField);
			}

			void MasterApplicationAdapter::OnTaskStart(opendnp3::TaskId id)
			{
				proxy->OnTaskStart(gcnew TaskId(id.id, id.isUserAssigned));
			}

			void MasterApplicationAdapter::OnTaskComplete(opendnp3::TaskId id, opendnp3::TaskCompletion result)
			{
				proxy->OnTaskComplete(gcnew TaskId(id.id, id.isUserAssigned), (TaskCompletion) result);
			}

			bool MasterApplicationAdapter::AssignClassDuringStartup()
			{
				return proxy->AssignClassDuringStartup();
			}

			void MasterApplicationAdapter::ConfigureAssignClassRequest(opendnp3::HeaderWriter& writer)
			{
				auto assignments = proxy->GetClassAssignments();
				for each(auto a in assignments)
				{										
					writer.WriteHeader(Convert(a.clazz), opendnp3::QualifierCode::ALL_OBJECTS);
					opendnp3::GroupVariationID id(a.group, a.variation);
					if (a.range.IsAllObjects())
					{
						writer.WriteHeader(id, opendnp3::QualifierCode::ALL_OBJECTS);
					}
					else
					{
						writer.WriteRangeHeader<openpal::UInt16>(opendnp3::QualifierCode::UINT16_START_STOP, id, a.range.start, a.range.stop);
					}
				}
			}

			opendnp3::GroupVariationID MasterApplicationAdapter::Convert(PointClass clazz)
			{
				switch (clazz)
				{
					case(PointClass::Class1) :
						return opendnp3::Group60Var2::ID();
					case(PointClass::Class2) :
						return opendnp3::Group60Var3::ID();
					case(PointClass::Class3) :
						return opendnp3::Group60Var4::ID();
					default:
						return opendnp3::Group60Var1::ID();
				}
			}
		}
	}
}