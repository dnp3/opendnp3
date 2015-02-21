#ifndef __OUTSTATION_DATABASE_ADAPTER_H_
#define __OUTSTATION_DATABASE_ADAPTER_H_

#include <opendnp3/outstation/IDatabase.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class OutstationDatabaseAdapter : Automatak::DNP3::Interface::IDatabase
			{
			public:

				OutstationDatabaseAdapter(opendnp3::IDatabase& proxy);

				virtual void Start();
				virtual void End();

				virtual bool Update(Automatak::DNP3::Interface::Binary^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::DoubleBitBinary^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::Analog^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::Counter^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::FrozenCounter^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::BinaryOutputStatus^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::AnalogOutputStatus^ meas, System::UInt16 index, EventMode mode);
				virtual bool Update(Automatak::DNP3::Interface::TimeAndInterval^ meas, System::UInt16 index);

				virtual bool Modify(System::Func<Binary^, Binary^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<DoubleBitBinary^, DoubleBitBinary^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<Analog^, Analog^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<Counter^, Counter^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<FrozenCounter^, FrozenCounter^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<BinaryOutputStatus^, BinaryOutputStatus^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<AnalogOutputStatus^, AnalogOutputStatus^>^ update, System::UInt16 index, EventMode mode);
				virtual bool Modify(System::Func<TimeAndInterval^, TimeAndInterval^>^ update, System::UInt16 index);
				
			private:

				asiodnp3::IMeasUpdater* pProxy;
			};

			
			struct FuncConverter
			{
				template <class Native, class Managed>
				static bool Invoke(opendnp3::IDatabase* pDatabase, System::Func<Managed^, Managed^>^ func, uint16_t index, opendnp3::EventMode mode)
				{
					gcroot<System::Func<Managed^, Managed^>^> root(func);

					auto apply = [&](const Native& value)
					{
						return Conversions::ConvertMeas(root->Invoke(Conversions::ConvertMeas(value)));
					};

					return pDatabase->Modify(openpal::Function1<const Native&, Native>::Bind(apply), index, mode);
				}

				template <class Native, class Managed>
				static bool Invoke(opendnp3::IDatabase* pDatabase, System::Func<Managed^, Managed^>^ func, uint16_t index)
				{
					gcroot<System::Func<Managed^, Managed^>^> root(func);

					auto apply = [&](const Native& value)
					{
						return Conversions::ConvertMeas(root->Invoke(Conversions::ConvertMeas(value)));
					};

					return pDatabase->Modify(openpal::Function1<const Native&, Native>::Bind(apply), index);
				}
			};
			
		}
	}
}

#endif
