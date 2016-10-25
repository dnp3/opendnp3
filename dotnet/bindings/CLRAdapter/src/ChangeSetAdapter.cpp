

#include "ChangeSetAdapter.h"
#include "Conversions.h"


namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			ChangeSetAdapter::ChangeSetAdapter() : changes(new asiodnp3::ChangeSet())
			{}

			ChangeSetAdapter::~ChangeSetAdapter()
			{
				delete changes;
			}

			void ChangeSetAdapter::Apply(asiodnp3::IOutstation& proxy)
			{
				proxy.Apply(*changes);				
			}

			void ChangeSetAdapter::Update(Binary^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Analog^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Counter^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(FrozenCounter^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				changes->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(TimeAndInterval^ update, System::UInt16 index)
			{
				changes->Update(Conversions::ConvertMeas(update), index);
			}			
		}
	}
}
