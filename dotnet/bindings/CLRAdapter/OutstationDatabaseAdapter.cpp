
#include "Stdafx.h"
#include "OutstationDatabaseAdapter.h"
#include "Conversions.h"

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{


			OutstationDatabaseAdapter::OutstationDatabaseAdapter(opendnp3::IDatabase& proxy) :
				pProxy(&proxy)
			{}

			void OutstationDatabaseAdapter::Start()
			{
				opendnp3::Transaction::Start(pProxy);
			}

			bool OutstationDatabaseAdapter::Update(Binary^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(DoubleBitBinary^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(Analog^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(Counter^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(FrozenCounter^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(BinaryOutputStatus^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(AnalogOutputStatus^ meas, System::UInt16 index, EventMode mode)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index, (opendnp3::EventMode) mode);
			}

			bool OutstationDatabaseAdapter::Update(TimeAndInterval^ meas, System::UInt16 index)
			{
				return pProxy->Update(Conversions::ConvertMeas(meas), index);
			}

			bool OutstationDatabaseAdapter::Modify(System::Func<Binary^, Binary^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::Binary, Binary>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<DoubleBitBinary^, DoubleBitBinary^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::DoubleBitBinary, DoubleBitBinary>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<Analog^, Analog^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::Analog, Analog>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<Counter^, Counter^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::Counter, Counter>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<FrozenCounter^, FrozenCounter^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::FrozenCounter, FrozenCounter>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<BinaryOutputStatus^, BinaryOutputStatus^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::BinaryOutputStatus, BinaryOutputStatus>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<AnalogOutputStatus^, AnalogOutputStatus^>^ update, System::UInt16 index, EventMode mode)
			{
				return FuncConverter::Invoke<opendnp3::AnalogOutputStatus, AnalogOutputStatus>(pProxy, update, index, (opendnp3::EventMode) mode);
			}
			
			bool OutstationDatabaseAdapter::Modify(System::Func<TimeAndInterval^, TimeAndInterval^>^ update, System::UInt16 index)
			{
				return FuncConverter::Invoke<opendnp3::TimeAndInterval, TimeAndInterval>(pProxy, update, index);
			}

			void OutstationDatabaseAdapter::End()
			{
				opendnp3::Transaction::End(pProxy);
			}


		}
	}
}
