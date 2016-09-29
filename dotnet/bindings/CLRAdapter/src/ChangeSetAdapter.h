#ifndef __CHANGESET_ADAPTER_H_
#define __CHANGESET_ADAPTER_H_

#include <asiodnp3/IOutstation.h>

#include <vcclr.h>

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{
			private ref class ChangeSetAdapter : public Automatak::DNP3::Interface::IDatabase
			{
			public:

				ChangeSetAdapter();

				~ChangeSetAdapter();
				
				virtual void Update(Binary^ update, System::UInt16 index, EventMode mode);
				virtual void Update(DoubleBitBinary^ update, System::UInt16 index, EventMode mode);				
				virtual void Update(Analog^ update, System::UInt16 index, EventMode mode);
				virtual void Update(Counter^ update, System::UInt16 index, EventMode mode);
				virtual void Update(FrozenCounter^ update, System::UInt16 index, EventMode mode);
				virtual void Update(BinaryOutputStatus^ update, System::UInt16 index, EventMode mode);
				virtual void Update(AnalogOutputStatus^ update, System::UInt16 index, EventMode mode);
				virtual void Update(TimeAndInterval^ update, System::UInt16 index);							

				void Apply(asiodnp3::IOutstation& proxy);
								
			private:
				
				asiodnp3::ChangeSet* changes;
			};
			
			struct FuncConverter
			{
				template <class Native, class Managed>
				static bool Invoke(asiodnp3::MeasUpdate* pUpdate, System::Func<Managed^, Managed^>^ func, uint16_t index, opendnp3::EventMode mode)
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
