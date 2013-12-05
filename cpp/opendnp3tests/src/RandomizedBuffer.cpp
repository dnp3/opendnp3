#include "RandomizedBuffer.h"

namespace opendnp3
{

RandomizedBuffer::RandomizedBuffer(size_t aSize) :
	CopyableBuffer(aSize),
	rand()
{
	Randomize();
}

void RandomizedBuffer::Randomize()
{
	for(size_t i = 0; i < this->Size(); ++i) mpBuff[i] = rand.Next();
}

}

