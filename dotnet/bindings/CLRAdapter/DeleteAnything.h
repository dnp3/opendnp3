#ifndef __DELETE_ANYTHING_H_
#define __DELETE_ANYTHING_H_

namespace DNP3
{
namespace Adapter
{

template <class T>
void DeleteAnything(T* apPointer)
{
	delete apPointer;
}

}
}

#endif
