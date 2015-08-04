

#include "ChangeSetAdapter.h"
#include "Conversions.h"


namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			ChangeSetAdapter::ChangeSetAdapter(asiodnp3::IOutstation& proxy) : 
				pProxy(&proxy),
				pUpdate(new asiodnp3::MeasUpdate(&proxy))
			{}

			ChangeSetAdapter::~ChangeSetAdapter()
			{
				delete pUpdate;
			}

			void ChangeSetAdapter::Apply()
			{
				if (pUpdate)
				{
					delete pUpdate;
					pUpdate = nullptr;
				}
			}

			void ChangeSetAdapter::Update(Binary^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Analog^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(Counter^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(FrozenCounter^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}
			
			void ChangeSetAdapter::Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index, (opendnp3::EventMode) mode);
			}

			void ChangeSetAdapter::Update(TimeAndInterval^ update, System::UInt16 index)
			{
				pUpdate->Update(Conversions::ConvertMeas(update), index);
			}			
		}
	}
}
