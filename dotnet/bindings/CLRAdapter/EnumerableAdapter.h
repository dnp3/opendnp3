
#ifndef __ENUMERABLE_ADAPTER_H_
#define __ENUMERABLE_ADAPTER_H_

#include <opendnp3/app/IterableBuffer.h>

#include "Conversions.h"

namespace DNP3
{
namespace Adapter
{

template <typename Target, typename Source>
public ref class EnumerableAdapter : System::Collections::Generic::IEnumerable<IndexedValue<Target>>
{
	public:

	EnumerableAdapter(const opendnp3::IterableBuffer<Source>& buffer) : pBuffer(&buffer)
	{}
		
	virtual System::Collections::IEnumerator^ GetEnumeratorNonGeneric() = System::Collections::IEnumerable::GetEnumerator
	{
		return GetEnumerator();
	}

	virtual System::Collections::Generic::IEnumerator<IndexedValue<Target>>^ GetEnumerator()
	{
		return gcnew EnumeratorAdapter<opendnp3::IterableBuffer<Source>::Iterator>(pBuffer->Iterate());
	}

	private:

	template <class Iterator>
	ref class EnumeratorAdapter : System::Collections::Generic::IEnumerator<IndexedValue<Target>>
	{
		public:

		EnumeratorAdapter(Iterator& iterator)
		{
			pIterator = new Iterator(iterator);
		}

		~EnumeratorAdapter()
		{
			delete pIterator;
		}

		virtual bool MoveNext()
		{
			return pIterator->MoveNext();
		}

		virtual void Reset()
		{
			return pIterator->Reset();
		}

		virtual property IndexedValue<Target> Current //= System::Collections::Generic::IEnumerator<Target>::Current
		{			
			IndexedValue<Target> get() sealed = System::Collections::Generic::IEnumerator<IndexedValue<Target>>::Current::get
			{
				auto current = pIterator->Current();
				auto meas = Conversions::ConvertMeas(current.value);

				return  IndexedValue
			}
		}

		// This is required as IEnumerator<T> also implements IEnumerator
		property Object^ Current2
		{
			virtual Object^ get() sealed = System::Collections::IEnumerator::Current::get
			{
				return this->Current;
			}
		};

		private:

		Iterator* pIterator;
	};

	const opendnp3::IterableBuffer<Source>* pBuffer;
};

}
}

#endif
