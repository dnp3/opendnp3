
#ifndef __ENUMERABLE_CONVERSIONS_H_
#define __ENUMERABLE_CONVERSIONS_H_

using namespace System::Collections::ObjectModel;

#include <opendnp3/app/IndexedValue.h>
#include <vcclr.h>

#include "Conversions.h"

using namespace Automatak::DNP3::Interface;

namespace Automatak
{
	namespace DNP3
	{
		namespace Adapter
		{

			template <class Target, class Source>
			static System::Collections::Generic::IEnumerable<IndexedValue<Target>^>^ ToEnumerable(
				const opendnp3::IterableBuffer<opendnp3::IndexedValue<Source, uint16_t>>& meas				
				)
			{
				auto list = gcnew System::Collections::Generic::List<IndexedValue<Target>^>();
				auto iterator = meas.Iterate();

				do
				{
					auto current = iterator.Current();
					auto value = gcnew IndexedValue<Target>(Conversions::ConvertMeas(current.value), current.index);
					list->Add(value);
				} while (iterator.MoveNext());

				return list;
			}
		}
	}
}

#endif
